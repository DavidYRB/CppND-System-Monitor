#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  
  Processor(): prevIdel{0}, prevTotal{0} {}
  friend class Process;

 private:
    float prevIdel, prevTotal;
};

#endif