#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  vector<float> memInfo;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line) && memInfo.size() < 2){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "MemTotal:"){
        memInfo.push_back(std::stoi(value));
        continue;
      }
      if(key == "MemFree:"){
        memInfo.push_back(std::stoi(value));
        continue;
      }
    }
  }
  return (memInfo[0] - memInfo[1]) / memInfo[0];
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line, uptime_s;
  long uptime_l{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_s;
    uptime_l = stof(uptime_s);
  }
  return uptime_l;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, key;
  long activeJiffies{0};
  const std::string kPid{to_string(pid)};
  std::ifstream stream(kProcDirectory + kPid + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 1; i < 14; ++i){
      linestream >> key;
    }
    for(int i = 14; i <= 17; ++i){
      linestream >> key;
      activeJiffies += std::stol(key);
    }
  }
  return activeJiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> cpuState = CpuUtilization();
  long activeTotal = std::stol(cpuState[LinuxParser::CPUStates::kUser_]) + 
                     std::stol(cpuState[LinuxParser::CPUStates::kNice_]) +
                     std::stol(cpuState[LinuxParser::CPUStates::kSystem_]) + 
                     std::stol(cpuState[LinuxParser::CPUStates::kSteal_]) + 
                     std::stol(cpuState[LinuxParser::CPUStates::kIRQ_]) + 
                     std::stol(cpuState[LinuxParser::CPUStates::kSoftIRQ_]);
  return activeTotal;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpuState = CpuUtilization();
  long idleTotal = std::stol(cpuState[LinuxParser::CPUStates::kIdle_]) + std::stol(cpuState[LinuxParser::CPUStates::kIOwait_]);
  return idleTotal;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuRecord;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "cpu"){
        while(linestream >> value){
          cpuRecord.push_back(value);
        }
        break;
      }
    }
  }
  return cpuRecord;
}

int LinuxParser::FindProcessInfo(std::string process_item){
  string line, key, key2, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == process_item){
        linestream >> value;
        return std::stoi(value);
      }
      if(key == "procs"){
        linestream >> key2 >> value;
        if((key2 == "running" && process_item == "procs_running") ||
           (key2 == "blocked" && process_item == "procs_blocked")){
             return std::stoi(value);
        }
      }
    }
  }
  return -1;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return FindProcessInfo("processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return FindProcessInfo("procs_running");
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  const std::string kPid{to_string(pid)};
  std::ifstream stream(kProcDirectory + kPid + kCmdlineFilename);
  std::cout <<"start command stream: " << kProcDirectory + std::to_string(pid) + kCmdlineFilename << '\n';
  if(stream.is_open()){
    std::cout <<"the stream is opened\n";
    std::getline(stream, line);
  }
  std::cout <<"cmd: " << line; 
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "VmSize:"){
        linestream >> value;
        break;
      }
    }
  }
  return value;
}

void LinuxParser::FindUidUser(std::unordered_map<string, string>& uidUserMap){
  string line;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      string user, mod, uid;
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> mod >> uid;
      if(uidUserMap.find(uid) == uidUserMap.end()){
        uidUserMap[uid] = user;
      }
    }
  }
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "uid:"){
        linestream >> value;
        std::cout << "Uid: " << value <<'\n';
        break;
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid, const std::unordered_map<string, string>& uidUserMap) {
  string uid = Uid(pid);
  auto it = uidUserMap.find(uid);
  return it->second;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, uptime_s;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 1; i <= 22; ++i){
      linestream >> uptime_s;
    }
  }
  std::cout << "process : " << pid << "uptime: " << uptime_s << '\n';
  long uptime_ticks = std::stol(uptime_s);
  return uptime_ticks/sysconf(_SC_CLK_TCK);
}