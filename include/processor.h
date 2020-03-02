#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  
  Processor(): prevIdel_{0}, prevTotal_{0} {}
  friend class Process;
  long TotalTime();

 private:
    long prevIdel_, prevTotal_;
};

#endif