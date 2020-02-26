#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long currIdel = LinuxParser::IdleJiffies();
    long currActive = LinuxParser::ActiveJiffies();
    long currTotal = currIdel + currActive;
    float totalDiff = currTotal - prevTotal;
    long idleDiff = currIdel - prevIdel;
    float cpuUsage = (totalDiff - idleDiff)/totalDiff;
    prevIdel = currIdel;
    prevTotal = currTotal;
    return cpuUsage;
}