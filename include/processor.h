#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
long _prevIdle=0;
long _idle=0;

long _prevActive=0;
long _active=0;

long _prevTotal=0;
long _total=0;

};

#endif