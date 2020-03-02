#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long currIdel = LinuxParser::IdleJiffies();
    long currActive = LinuxParser::ActiveJiffies();
    long currTotal = currIdel + currActive;
    float totalDiff = currTotal - prevTotal_;
    long idleDiff = currIdel - prevIdel_;
    float cpuUsage = (totalDiff - idleDiff)/totalDiff;
    prevIdel_ = currIdel;
    prevTotal_ = currTotal;
    return cpuUsage;
}

long Processor::TotalTime(){
    return prevTotal_;
}