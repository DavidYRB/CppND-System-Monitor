#include <unistd.h>
#include <cstddef>
#include <queue>
#include <string>
#include <set>
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
    if(processes_.size() != 0){
        processes_.erase(processes_.begin(), processes_.end());
    }
    vector<int> pids = LinuxParser::Pids();
    std::unordered_map<std::string, std::string> uidUserMap;
    LinuxParser::FindUidUser(uidUserMap);
    auto cmp = [](Process& a, Process& b) {return a < b;};
    std::priority_queue<Process, vector<Process>, decltype(cmp)> process_queue(cmp);
    vector<int>::iterator pidIt;
    for(pidIt = pids.begin(); pidIt != pids.end(); pidIt++){
        // if(processesMap_.find(*pidIt) == processesMap_.end()){
        //     Process processTemp(*pidIt, uidUserMap);
        // }
        // else{
        //     processesMap_[*pidIt].UpdateProcess();
        // }
        // process_queue.push(processesMap_[*pidIt]);
        Process processTemp(*pidIt, uidUserMap);
        process_queue.push(processTemp);
    }
    while(!process_queue.empty()){
        processes_.push_back((process_queue.top()));
        process_queue.pop();
    }
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