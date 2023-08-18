/**
 * @file dumbbell.cpp
 * @author Souta Miyamoto (miyamoon98@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include<iostream>
#include<cmath>
#include "rand.h"

#include"dumbbell.h"

namespace dumbbell{



sys::sys()
{
  this->x = nullptr;
}
//
sys::sys(std::vector<int> N_,
      std::vector<double> tau_,
      std::vector<double> max_ell_, 
      double              G0_ )
{
  this->x = nullptr;
  this->init(N_,tau_,max_ell_,G0_);
}
//
sys::~sys()
{
  this->destroy();
}
//
void sys::destroy()
{
  if( x != nullptr ) {
    delete[] x;
  }
  return;
}
//
void sys::init(std::vector<int> N_,
          std::vector<double> tau_,
          std::vector<double> max_ell_, 
          double              G0_ )
{  
  this->destroy();
  std::copy(N_.begin(), N_.end(), std::back_inserter(N));
  std::copy(tau_.begin(), tau_.end(), std::back_inserter(tau));
  std::copy(max_ell_.begin(), max_ell_.end(), std::back_inserter(max_ell));
  this->Ntotal = 0;
  this->G0 = G0_;
  for(size_t i = 0; i < N.size(); i++){
    Ntotal += N[i];
  }
  this->x = new vec3d [Ntotal];

  int k = 0;
  for(int i=0; i<N.size(); i++) {
    const double f = std::sqrt(1.0/3.0);
    for(int j=0; j<N[i]; j++) {
      x[k][0] = mrand::nrand()*f;
      x[k][1] = mrand::nrand()*f;
      x[k][2] = mrand::nrand()*f;
      k++;      
    }
  }
  return;
}
//
void sys::update(const double dt,const double* vel_grad)
{
  using mrand::nrand;
  int k = 0;
  for(int i=0; i<N.size(); i++) {
    const double f1 = 0.5/tau[i];
    const double f2 = std::sqrt(1.0/(3.0*dt*tau[i]));
    for(int j=0; j<N[i]; j++) {
      vec3d& x_ = x[k];
      double lensq = x_[0]*x_[0] + x_[1]*x_[1] + x_[2]*x_[2];
      double fene_f = (max_ell[i] > 0.0)? 1.0/(1.0-lensq/(max_ell[i]*max_ell[i])):1.0;
      double w_[3] = {nrand(),nrand(),nrand()};
      const double dx_x = (vel_grad[3*0+0]*x_[0]+vel_grad[3*0+1]*x_[1]+vel_grad[3*0+2]*x_[2]) - fene_f*f1*x_[0] + f2*w_[0];
      const double dx_y = (vel_grad[3*1+0]*x_[0]+vel_grad[3*1+1]*x_[1]+vel_grad[3*1+2]*x_[2]) - fene_f*f1*x_[1] + f2*w_[1];
      const double dx_z = (vel_grad[3*2+0]*x_[0]+vel_grad[3*2+1]*x_[1]+vel_grad[3*2+2]*x_[2]) - fene_f*f1*x_[2] + f2*w_[2];
      x_[0] += dx_x*dt;
      x_[1] += dx_y*dt;
      x_[2] += dx_z*dt;
      k++;
    }
  }
  return;
}
//
void sys::eval_orientation(double* orientation)
{
  for (int i = 0; i < 6; i++)
    orientation[i] = 0.0;
  int k = 0;
  for(int i=0; i<N.size(); i++) {
    for(int j=0; j<N[i]; j++) {
      const vec3d& x_ = x[k];
      double invlensq = 1.0/(x_[0]*x_[0]+x_[1]*x_[1]+x_[2]*x_[2]);
      orientation[0] += x_[0]*x_[0]*invlensq;
      orientation[1] += x_[1]*x_[1]*invlensq;
      orientation[2] += x_[2]*x_[2]*invlensq;
      orientation[3] += x_[0]*x_[1]*invlensq;
      orientation[4] += x_[1]*x_[2]*invlensq;
      orientation[5] += x_[2]*x_[0]*invlensq;
      k++;
    }
  }
  const double inv_N = 1.0/Ntotal;
  for (int i = 0; i < 6; i++)
    orientation[i] *= inv_N;
  return;
}
//
void sys::eval_stress(double* stress)
{
  for (int i = 0; i < 6; i++)
    stress[i] = 0.0;
  int k = 0;
  for(int i=0; i<N.size(); i++) {
    for(int j=0; j<N[i]; j++) {
      const vec3d& x_ = x[k];
      double lensq = x_[0]*x_[0]+x_[1]*x_[1]+x_[2]*x_[2];
      double fene_f = (max_ell[i] > 0.0)? 1.0/(1.0-lensq/(max_ell[i]*max_ell[i])):1.0;
      stress[0] += 3.0*fene_f*x_[0]*x_[0] - 1.0;
      stress[1] += 3.0*fene_f*x_[1]*x_[1] - 1.0;
      stress[2] += 3.0*fene_f*x_[2]*x_[2] - 1.0;
      stress[3] += 3.0*fene_f*x_[0]*x_[1]      ;
      stress[4] += 3.0*fene_f*x_[1]*x_[2]      ;
      stress[5] += 3.0*fene_f*x_[2]*x_[0]      ;
      k++;
    }
  }
  const double inv_N = 1.0/Ntotal;
  for (int i = 0; i < 6; i++)
    stress[i] *= this->G0 * inv_N;
  return ;
}
//
void sys::dump(gsd_handle* gh) const
{
  const int Ns = N.size();
  gsd_write_chunk(gh,"Ntotal" ,gsd_type::GSD_TYPE_INT32 ,1     ,1,0,&this->Ntotal       ); 
  gsd_write_chunk(gh,"Ns"     ,gsd_type::GSD_TYPE_INT32 ,1     ,1,0,&Ns                 );
  gsd_write_chunk(gh,"N"      ,gsd_type::GSD_TYPE_INT32 ,Ns    ,1,0,this->N.data()      );
  gsd_write_chunk(gh,"tau"    ,gsd_type::GSD_TYPE_DOUBLE,Ns    ,1,0,this->tau.data()    );
  gsd_write_chunk(gh,"max_ell",gsd_type::GSD_TYPE_DOUBLE,Ns    ,1,0,this->max_ell.data());
  gsd_write_chunk(gh,"x"      ,gsd_type::GSD_TYPE_DOUBLE,Ntotal,3,0,&(this->x[0][0])    );  
  gsd_end_frame(gh);
  return;
};

void sys::restart(gsd_handle* gh, const int frame)
{
  gsd_read_chunk(gh,&this->Ntotal,gsd_find_chunk(gh,frame,"Ntotal"));
  int Ns;
  gsd_read_chunk(gh,&Ns,gsd_find_chunk(gh,frame,"Ns"));
  this->N.resize(Ns);
  this->tau.resize(Ns);
  this->max_ell.resize(Ns);
  gsd_read_chunk(gh,N.data()      ,gsd_find_chunk(gh,frame,"N"      ));
  gsd_read_chunk(gh,tau.data()    ,gsd_find_chunk(gh,frame,"tau"    ));
  gsd_read_chunk(gh,max_ell.data(),gsd_find_chunk(gh,frame,"max_ell"));
  gsd_read_chunk(gh,&(x[0][0])    ,gsd_find_chunk(gh,frame,"x"      ));
  return;
};


}// namespace