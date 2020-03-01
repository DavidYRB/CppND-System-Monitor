#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

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
    os_name_(System::RetrieveOSName()),
    kernelVersion_(System::RetrieveKernel()){}

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.erase(processes_.begin(), processes_.end());
    vector<int> pids = LinuxParser::Pids();
    LinuxParser::FindUidUser(uidUserMap_);
    vector<int>::iterator pidIt;
    for(pidIt = pids.begin(); pidIt != pids.end(); pidIt++){
        Process processTemp(*pidIt, this);
        if(processes_.empty() || *processes_.begin() < processTemp){
            processes_.insert(processes_.begin(), processTemp);
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

string System::RetrieveOSName(){
    return LinuxParser::OperatingSystem();
}

string System::RetrieveKernel(){
    return LinuxParser::Kernel();
};