#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{
    long hours = seconds/3600l;
    seconds -= hours*3600l;
    long minutes = seconds/60l;
    seconds -= minutes*60l;
    return std::to_string(hours) + std::to_string(minutes) + std::to_string(seconds);

}