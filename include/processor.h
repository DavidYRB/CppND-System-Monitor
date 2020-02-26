#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor(): prevIdel{0}, prevTotal{0} {}

  // TODO: Declare any necessary private members
 private:
    float prevIdel, prevTotal;
};

#endif