#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// TODO: Return this process's ID
Process::Process(int pid) : _pid(pid), proccessor(pid) {}

int Process::Pid() 
{
    return _pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    return proccessor.Utilization();
}

// TODO: Return the command that generated this process
string Process::Command() 
{
    return LinuxParser::Command(_pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() const
{
    return LinuxParser::Ram(_pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{
    return LinuxParser::User(_pid); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{
    return LinuxParser::UpTime(_pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{
    return (Ram() < a.Ram());
}