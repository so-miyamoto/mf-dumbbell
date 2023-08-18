/**
 * @file timer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<ctime>
#include<iostream>

#include"timer.h"

using namespace std::chrono;

void
ChronoTimer::start(){
  _start = system_clock::now();
}
void 
ChronoTimer::end(){
  _end = system_clock::now();
}

double
ChronoTimer::elapsed_msec(){
  return duration_cast<milliseconds>(_end-_start).count();
}

double
ChronoTimer::elapsed_sec(){
  return duration_cast<seconds>(_end-_start).count();
}

double
ChronoTimer::elapsed_microsec(){
  return duration_cast<microseconds>(_end-_start).count();
}

void
ChronoTimer::stamp_time(){
  std::time_t t = system_clock::to_time_t(system_clock::now());
  std::cout << std::ctime(&t) <<std::endl;
}
