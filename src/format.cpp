#include <string>
#include <sstream>
#include <iomanip>
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

    std::ostringstream osstream;
    osstream << std::setw(2) << std::setfill('0') << hours 
    << ":" << std::setw(2) << std::setfill('0') << minutes 
    << ":" << std::setw(2) << std::setfill('0') << seconds;

    return osstream.str();

}