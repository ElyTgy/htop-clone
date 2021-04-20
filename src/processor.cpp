#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
     _prevIdle = _idle;
     _prevActive = _active;
     _prevTotal = _total;

     _idle = LinuxParser::IdleJiffies();
     _active = LinuxParser::ActiveJiffies();
     _total = LinuxParser::Jiffies();

    long totald = _total - _prevTotal;
    long idled = _idle - _prevIdle;

    return (totald-idled)/totald;

}