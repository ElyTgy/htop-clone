#include "processor.h"
#include <cassert>
#include <iostream>

float ProcessorBase::Utilization()
{
    return 0.5f;
    _prevActive = _active;
    _prevTotal = _total;
    _active = GetActiveJiffies();
    _total = GetActiveJiffies();

    if(_total != _prevTotal && _active != _prevActive)
    {
        _totald = float(_total) - float(_prevTotal);
        _actived = float(_active) - float(_prevActive);
        float addPercentage = _actived/_totald;
        _percentage += addPercentage;

        //std::cout<<"active: "<<_active<<std::endl;
        //std::cout<<"total: "<<_total<<std::endl;
        //std::cout<<"totald: "<<_totald<<std::endl;
        //std::cout<<"actived: " <<_actived<<std::endl;
        //std::cout<<std::endl<<"Active to total ratio: "<<_percentage<<std::endl;
    }

    return _percentage;
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