#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

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
  return 100.0f - (memInfo[MemFree] * 100.0f / memInfo[MemTotal]); 
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
{ return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

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
string LinuxParser::Ram(int pid[[maybe_unused]]) 
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
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) {return 0;}

int LinuxParser::GetValueForField(const std::string& filePath, const std::string& fieldName)
{
    std::ifstream file (filePath);
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
	  			int fieldVal;
	  			linestream >> fieldVal;
	  			return fieldVal;
	  		}
	  	}
	  }
}
