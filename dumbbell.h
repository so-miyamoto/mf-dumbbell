/**
 * @file dumbbell.h
 * @author Souta Miyamoto (miyamoon98@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include<vector>

#include "gsd.h"


namespace dumbbell{

typedef double vec3d[3];

//
struct sys {
  int Ntotal;
  std::vector<int> N;      // bonds
  std::vector<double> tau; // relaxation time
  std::vector<double> max_ell; // maximum length 
  double G0;     // relaxation modulus
  vec3d* x;     // bond vector

public:
  sys();
  sys(std::vector<int> N_,
      std::vector<double> tau_,
      std::vector<double> max_ell_,
      double              G0_ );
  ~sys();
  void init(std::vector<int> N_,
            std::vector<double> tau_,
            std::vector<double> max_ell_, 
            double              G0_ );
  void eval_orientation(double* orientation);
  void eval_stress(double* stress);
  void update(const double dt,const double* vel_grad);
  void dump(gsd_handle* gh) const;
  void restart(gsd_handle* gh, const int frame);
private:
  void destroy();

};



}; // namespace