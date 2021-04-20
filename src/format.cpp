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

    std::string secondsstr = (seconds >= 0) ? (std::to_string(seconds)) : ("00");
    std::string minutesstr = (minutes >= 0) ? (std::to_string(minutes)) : ("00");
    std::string hoursstr = (hours >= 0) ? (std::to_string(hours)) : ("00");

    if(hoursstr.length() == 1){hoursstr = "0"+hoursstr;}
    if(minutesstr.length() == 1){minutesstr = "0"+minutesstr;}
    if(secondsstr.length() == 1){secondsstr = "0"+secondsstr;}

    return hoursstr+":"+minutesstr+":"+secondsstr;

}