#include "processor.h"
#include <cassert>
#include <iostream>
#include <math.h>

float ProcessorBase::Utilization()
{
    _prevActive = _active;
    _prevTotal = _total;
    _prevIdle = _idle;

    _idle = LinuxParser::IdleJiffies();
    _active = GetActiveJiffies();
    _total = _idle + _active;

    float _totald = float(_total) - float(_prevTotal);
    float _idled = float(_idle) - float(_prevIdle);
    if(_totald <= epsilon){return 0.0f;}
    return (_totald - _idled)/_totald;
}

CPUProccessor::CPUProccessor(int cpuNum):_cpuNum(cpuNum){}
long CPUProccessor::GetActiveJiffies()
{
     return LinuxParser::ActiveJiffies();
}

ProccessProccessor::ProccessProccessor(int pid) : _processNum(pid){}
long ProccessProccessor::GetActiveJiffies()
{
     return LinuxParser::ActiveJiffies(_processNum);
}