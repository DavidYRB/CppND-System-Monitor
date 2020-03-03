#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::unordered_map<std::string, std::string>& uidUser): pid_(pid){
    command_ = LinuxParser::Command(pid_);
    user_ = LinuxParser::User(pid_, uidUser);
    uptime_ = LinuxParser::UpTime(pid_);
    cpuUsage_ = AverageCpuUsage();
    string ram = LinuxParser::Ram(pid_);
    long ram_value = std::stol(ram);
    ram_ = to_string(ram_value/1024);

}

// Return this process's ID
int Process::Pid() {
    return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    return cpuUsage_;
}

// Return the command that generated this process
string Process::Command() { 
    return command_;
}

// Return this process's memory utilization
string Process::Ram() {
    return ram_;
}

// Return the user (name) that generated this process
string Process::User() {
    return user_;
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
    return uptime_;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(const Process& a) const{
    return this->cpuUsage_ < a.cpuUsage_ ? true : false;
}

bool Process::operator==(const Process& a) const{
    return this->pid_ == a.pid_;
}

float Process::AverageCpuUsage(){
    long cpuUptime = LinuxParser::UpTime();
    long processActiveJiffies = LinuxParser::ActiveJiffies(pid_);
    return processActiveJiffies * 1.0 / (uptime_ * sysconf(_SC_CLK_TCK));
}

float Process::UpdateCpuUsage(){
    long currActiveJiffies = LinuxParser::ActiveJiffies(pid_);
    long currUptime = LinuxParser::UpTime(pid_);
    long jiffesDiff = currActiveJiffies - activeJiffies_;
    if(jiffesDiff < 0){
        jiffesDiff = 0;
    }
    float processCpuUsage = jiffesDiff * 1.0 / ((currUptime - uptime_) *sysconf(_SC_CLK_TCK));
    activeJiffies_ = currActiveJiffies;
    uptime_ = currUptime;
    return processCpuUsage;
}

void Process::UpdateProcess(){
    cpuUsage_ = UpdateCpuUsage();
    ram_ = LinuxParser::Ram(pid_);
}