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
    
}

// Return this process's ID
int Process::Pid() {
    std::cout <<"Get PID: " << pid_ <<'\n';
    return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    std::cout <<"Get CpuUsage\n";
    return AverageCpuUsage();
}

void Process::CpuTimePrev(long time){
    cpuTimePrev_ = time;
}

// Return the command that generated this process
string Process::Command() { 
    std::cout <<"Get Command\n";
    return command_;
}

// Return this process's memory utilization
string Process::Ram() {
    std::cout <<"Get Ram\n";
    string ram = LinuxParser::Ram(pid_);
    std::cout << "Ram: " << ram << '\n';
    long ram_value = std::stol(ram);
    ram = to_string(ram_value/1024);
    return ram;
}

// Return the user (name) that generated this process
string Process::User() {
    std::cout <<"Get User\n";
    return user_;
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
    std::cout <<"Get Uptime\n";
    return uptime_;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(const Process& a) const{
    return this->cpuUsage_ < a.cpuUsage_ ? true : false;

}

float Process::AverageCpuUsage(){
    long cpuUptime = LinuxParser::UpTime();
    long processTime = cpuUptime - uptime_;
    long processActiveJiffies = LinuxParser::ActiveJiffies(pid_);
    return processActiveJiffies * 1.0 / (processTime * sysconf(_SC_CLK_TCK));
}

float Process::LatestCpuUsage(){
    long activeJiffies = LinuxParser::ActiveJiffies(pid_);
    long cpuTime = LinuxParser::Jiffies();
    float processCpuUsage = (activeJiffies - activeJiffiesPrev_) * 1.0 / (cpuTime - cpuTimePrev_);
    activeJiffiesPrev_ = activeJiffies;
    return processCpuUsage;
}