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
  // The safety concern, should the linuxParser have access to the private member of system?
  friend void LinuxParser::User(int pid, const std::unordered_map<std::string, std::string>& uidUserMap);
  friend class Process;
  
 private:
  const std::string kernelVersion_;
  const std::string os_name_;
  Processor cpu_;
  std::vector<Process> processes_;
  std::unordered_map<std::string, std::string> uidUserMap_;

  std::string RetrieveKernel();
  std::string RetrieveOSName();

};

#endif