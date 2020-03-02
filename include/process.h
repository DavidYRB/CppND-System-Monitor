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
  Process(int pid, std::unordered_map<std::string, std::string>& uidUser);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(const Process& a) const;  // TODO: See src/process.cpp
  void CpuTimePrev(long time);
  // Declare any necessary private members
 private:
  int pid_;
  long cpuTimePrev_;
  std::string user_;
  std::string command_; 
  long activeJiffiesPrev_;
  float cpuUsage_;
  float ramUsage_;
  long int uptime_;

  float AverageCpuUsage();
  float LatestCpuUsage();
};

#endif