#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
//MemTotal - MemFree in percentage
float LinuxParser::MemoryUtilization() 
{
  enum mem {MemTotal,MemFree,TotalFields};

  string line;
  string field;
  long unsigned int memInfo[TotalFields]; 

  std::ifstream file(std::ifstream(kProcDirectory + kMeminfoFilename));
  if(file.is_open())
  {
      for(int i = 0; i < TotalFields; ++i)
      {
          std::getline(file, line);
          std::istringstream linestream(line);
          linestream >> field >> memInfo[i];
      }
  }
  return 100.0 - (memInfo[MemFree] * 100.0 / memInfo[MemTotal]); 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
    std::ifstream file(kProcDirectory + kUptimeFilename);
    long upTime;
    
    if(file.is_open())
    {
      file >> upTime;
    }
    return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{
    std::vector<std::string> cpuUtils(CpuUtilization());
    std::vector<long> cpuUtilsl{};
    
    for(unsigned int i = 0; i < cpuUtils.size(); ++i)
    {
      cpuUtilsl.push_back(stol(cpuUtils[i]));
    }

    cpuUtilsl[kUser_] = cpuUtilsl[kUser_] - cpuUtilsl[kGuest_];
    cpuUtilsl[kNice_] = cpuUtilsl[kNice_] - cpuUtilsl[kGuestNice_];

    long sum = 0;
    for(int i = 0; i < CPUStatesNum_; ++i)
    {
        sum += cpuUtilsl[i];
    }

    return sum;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{
    std::string filename = (kProcDirectory+to_string(pid)+kStatFilename);
    long utime = LinuxParser::GetValueAtRow(filename, fieldnum_utime);
    long stime = LinuxParser::GetValueAtRow(filename, fieldnum_stime);
    
    return (long long int)utime +  stime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
    return Jiffies()-IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
    std::vector<std::string> cpuUtils(CpuUtilization());
    return stol(cpuUtils[kIdle_])+stol(cpuUtils[kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{
    return GetValuesForField(kProcDirectory+kStatFilename, field_cpu);
}

//returns the CPU utililzation for a specific CPU
vector<string> LinuxParser::CpuUtilization(int n) 
{
    return GetValuesForField(kProcDirectory+kStatFilename, field_cpu+to_string(n));
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  return GetValueForField(kProcDirectory + kStatFilename, 
                            field_totalProccesses);
} 

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  return GetValueForField(kProcDirectory + kStatFilename, 
                            field_runningProccesses);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
    std::ifstream file(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    std::string cmd;
    file>>cmd;
    return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
    return to_string(GetValueForField(kProcDirectory + std::to_string(pid) + kStatusFilename,
                            field_pidVMSize));
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
    return to_string(GetValueForField(kProcDirectory + std::to_string(pid) + kStatusFilename,
                            field_pidUID));
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
    std::fstream file(kPasswordPath);
    if(file.is_open())
    {
        std::string uid = Uid(pid);
        std::string findStr = uid + ':' + uid + ':';
        std::string line;

        while(getline(file, line))
        {
            if(line.find(findStr) != string::npos)
            {
                std::replace(line.begin(), line.end(), ':', ' ');
                std::istringstream lineisstream(line);
                std::string username;
                lineisstream >> username;
                return username;
            }
        }
        assert(!"Couldnt find user");
    }
    else{assert(!"couldnt open file in user function");}
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
    long int uptime = GetValueAtRow(kProcDirectory + to_string(pid) + kStatFilename, fieldnum_username);
    return uptime / sysconf(_SC_CLK_TCK);
}

int LinuxParser::GetValueForField(const std::string& filePath, const std::string& fieldName)
{
    std::ifstream file (filePath);
    int fieldVal=0;
	  if (file.is_open())
	  {
	    	std::string line;
	    	while (std::getline(file, line))
	  	  {
	  		    std::string field;
	  		    file >> field;
	  		    if (field == fieldName)
	  		    {
	  			      file >> fieldVal;
	  			      return fieldVal;
	  		    }
	  	  }
        assert(!"Invlaid field name in GetValueForField");
	  }
    else{assert(!"Coudlnt open file in GetValueForField");}
    return fieldVal;//the compiler will complain if i dont return something eventhough this cant be returned
}

long int LinuxParser::GetValueAtRow(const std::string& filePath, int rowNum)
{
    std::ifstream file(filePath);
    long val=0;
    if(file.is_open())
    {
        for(int i = 1; i <= rowNum; ++i)
        {
            file>>val;
        }
        return val;
        assert(!"row doesnt exist");
    }
    else{assert(!"Cant open file");}
    return val;//the compiler will complain if i dont return something eventhough this cant be returned
}

std::vector<std::string> LinuxParser::GetValuesForField(const std::string& filePath, const std::string& fieldName)
{
    std::ifstream file (filePath);
    std::vector<std::string> vecOfValues{};
	  if (file.is_open())
	  {
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream linestream (line);
            std::string field;
            linestream >> field;
            if (field == fieldName)
            {
                std::string currentVal;
                while(linestream >> currentVal)
                {
                    vecOfValues.push_back(currentVal);
                }
                return vecOfValues;
            }
            assert("Couldnt find the right field in 'GetValuesForField'");
        }
	  }
    else{assert(!"Couldnt open file in 'GetValueForFields'");}
    return vecOfValues;//the compiler will complain if i dont return something eventhough this cant be returned
}

