#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

struct ProcessorBase {
public:
  float Utilization();
  virtual long GetActiveJiffies()=0;
protected:
    float _totald=0;
    float _idled=0;

    long _prevIdle=0;
    long _idle=0;

    long _prevActive=0;
    long _active=0;

    long _prevTotal=0;
    long _total=0;
};

class CPUProccessor : public ProcessorBase
{
//currently only calculates the aggregate cpu's utilization
public:
    CPUProccessor(int cpuNum=-1); //-1 means aggregate cpu proccess
    long GetActiveJiffies() override;
private:
    const int _cpuNum;
};

class ProccessProccessor : public ProcessorBase
{
public:
    ProccessProccessor(int pid);
    long GetActiveJiffies() override;
private:
    const int _processNum;
};

#endif
