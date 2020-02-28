#ifndef SYSTEM_H
#define SYSTEM_H

#include <unordered_map>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  const std::string kernelVersion_;
  const std::string os_name_;
  Processor cpu_;
  std::vector<Process> processes_;
  std::unordered_map<string, string> uidUserMap_;

  std::string RetrieveKernel();
  std::string RetrieveOSName();

};

#endif