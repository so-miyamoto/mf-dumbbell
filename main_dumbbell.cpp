
#include <iostream>
#include <exception>
#include <vector>
#include <cmath>
#include <random>

#include <yaml-cpp/yaml.h>

#include "dumbbell.h"
#include "timer.h"

#include "rand.h"

int main(int argc, char** argv) 
{
  try{

  std::random_device rng;
  mrand::init(rng());

  std::string yaml_paramf(argv[1]); 

  auto node = YAML::LoadFile(yaml_paramf);
  auto numof_dumbbells    = node["system"]["numof_dumbbells"].as<std::vector<int>>();
  auto relaxation_time    = node["system"]["relaxation_time"].as<std::vector<double>>();
  auto maximum_length     = node["system"]["maximum_length" ].as<std::vector<double>>();
  auto relaxation_modulus = node["system"]["relaxation_modulus"].as<double>();
  
  dumbbell::sys* sys = new dumbbell::sys(numof_dumbbells,relaxation_time,maximum_length,relaxation_modulus);

  auto dt       = node["integrate"]["dt"].as<double>();
  auto max_step = node["integrate"]["max_step"].as<int>();
  auto velgrad  = node["integrate"]["velgrad"].as<std::vector<double>>();
  auto angvel   = node["integrate"]["angvel"].as<double>();  
  auto duration = node["integrate"]["duration"].as<int>();

  ChronoTimer timer;
  timer.start();

  for (int step = 0; step < max_step; step++) {
    double velgrad_ontime[9];
    double time = step*dt;
    for (int i = 0; i < 9; i++)
      velgrad_ontime[i] = std::cos(angvel*time)*velgrad[i];

    if( step % duration == 0 ) {
      double stress[6];
      sys->eval_stress(stress);     

      sys->update(dt,velgrad_ontime);

      double stress_n[6];
      sys->eval_stress(stress_n);

      for (size_t i = 0; i < 9; i++)    
        std::cout << velgrad_ontime[i] << " ";
      for (size_t i = 0; i < 6; i++)    
        std::cout << stress[i] << " ";
      for (size_t i = 0; i < 6; i++)    
        std::cout << ((stress_n[i]-stress[i])/dt) << " ";
      std::cout << "\n";
    } else {
      sys->update(dt,velgrad_ontime);
    }
    //
  }

  timer.end();
  std::cerr << timer.elapsed_msec() << " [msec]"<< std::endl;

  delete sys;

  }catch(YAML::Exception &e) {
    std::cout << "YAML Error!" << std::endl;
    std::cout << e.what() << std::endl;;
    return 1;
  }catch(std::runtime_error &e) {
    std::cout << "Runtime Error!" << std::endl;;
    std::cout << e.what() << std::endl;;
    return 1;
  }catch(std::logic_error &e) {
    std::cout << "Logic Error!" << std::endl;;
    std::cout << e.what() << std::endl;;
    return 1;
  }catch(...) {
    std::cout << "Unknown Error!" << std::endl;;
    return 9;
  }
}