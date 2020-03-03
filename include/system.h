#ifndef SYSTEM_H
#define SYSTEM_H

#include <unordered_map>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();
  
 private:
  const std::string kernelVersion_;
  const std::string os_name_;
  Processor cpu_;
  std::vector<Process> processes_;
  std::unordered_map<int, Process> processesMap_;

};

#endif