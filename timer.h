/**
 * @file timer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include<chrono>

class ChronoTimer{

  std::chrono::system_clock::time_point _start;
  std::chrono::system_clock::time_point _end;

public:
  void
  start();
  void 
  end();

  double
  elapsed_msec();

  double
  elapsed_sec();

  double
  elapsed_microsec();

  void
  stamp_time();

};

