#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "system.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, System* hostSystem);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  long activeJiffiesPrev_;
  float cpuUsage_;
  float ramUsage_;
  long int uptime_;
  const int pid_;
  const std::string user_;
  const std::string command_; 
  const System* hostSystem_;
};

#endif