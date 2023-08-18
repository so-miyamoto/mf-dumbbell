/**
 * @file rand.h
 * @author Souta Miyamoto (miyamoon98@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-27
 */
#include<cmath>
#include<random>

#ifdef _OPENMP
# include <omp.h>
#endif

#include"dSFMT.h"


namespace mrand{

  void init(const unsigned int seed);
  void finalize();
  void gen_nrand(double* array,const int size);
  double nrand();
  double rand(); 

}// namespace
