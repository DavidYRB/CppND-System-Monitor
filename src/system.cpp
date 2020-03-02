#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <iostream>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Construct the system object
System::System() : 
    os_name_(LinuxParser::OperatingSystem()),
    kernelVersion_(LinuxParser::Kernel()){}

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
    //std::cout <<"startd getting process\n";
    if(processes_.size() != 0){
        processes_.erase(processes_.begin(), processes_.end());
    }
    //std::cout <<"Get all pids\n";
    vector<int> pids = LinuxParser::Pids();
    //std::cout <<"Find uid user map\n";
    std::unordered_map<std::string, std::string> uidUserMap;
    LinuxParser::FindUidUser(uidUserMap);
    //std::cout <<"uidUserMap size: " << uidUserMap.size() << '\n';
    vector<int>::iterator pidIt;
    //std::cout <<"Start to put processes in order\n";
    for(pidIt = pids.begin(); pidIt != pids.end(); pidIt++){
        //std::cout <<"PID: " << *pidIt << '\n';
        Process processTemp(*pidIt, uidUserMap);
        //std::cout <<"Created temp process object\n";
        processTemp.CpuTimePrev(Cpu().TotalTime());
        if(processes_.empty()){
            //std::cout <<"vector is empty\n";
            processes_.push_back(processTemp);
            continue;
        }
        else if(*(processes_.begin()) < processTemp){
            processes_.insert(processes_.begin(), processTemp);
            //std::cout <<"Insert at head\n";
            continue;
        }
        for(auto processIt = processes_.begin(); processIt != processes_.end(); processIt++){
            if(processTemp < *processIt){
                if(*(processIt + 1) < processTemp || processIt + 1 == processes_.end()){
                    processes_.insert(processIt + 1, processTemp);
                }
            }
        }
    }
    std::cout <<"Finished process parsing\n";
    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() {
    return kernelVersion_;
}

// Return the system's memory utilization
float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return os_name_;
}

// Return the number of processes actively running on the system
int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime();
}