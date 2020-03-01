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

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// Return the command that generated this process
string Process::Command() { 
    return command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// Return the user (name) that generated this process
string Process::User() {
    return user_;
}

// TODO: Return the age of this process (in seconds)
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