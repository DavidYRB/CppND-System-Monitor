#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, System* hostSystem) : pid_(pid),
                            hostSystem_(hostSystem),
                            user_(LinuxParser::User(pid, hostSystem->uidUserMap_)), 
                            command_(LinuxParser::Command(pid)){}

// Return this process's ID
int Process::Pid() {
    return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    long activeJiffies = LinuxParser::ActiveJiffies(pid_);
    long cpuTime = LinuxParser::Jiffies();
    float processCpuUsage = (activeJiffies - activeJiffiesPrev_) * 1.0 / (cpuTime - hostSystem_->cpu_.prevTotal);
    activeJiffiesPrev_ = activeJiffies;
    return processCpuUsage;
}

// Return the command that generated this process
string Process::Command() { 
    return command_;
}

// Return this process's memory utilization
string Process::Ram() {
    string ram = LinuxParser::Ram(pid_);
    long ram_value = std::stol(ram);
    ram = to_string(ram_value/1024);
    return ram;
}

// Return the user (name) that generated this process
string Process::User() {
    return user_;
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    if(a.cpuUsage_ != this.cpuUsage_){
        return this.cpuUsage_ < a.cpuUsage_ ? true : false;
    }
    else{
        return this.ramUsage_ <= a.ramUsage_ ? true : false;
    }
}