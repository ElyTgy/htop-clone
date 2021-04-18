#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};
//Accessed fields in files
const std::string field_totalProccesses = "processes";
const std::string field_runningProccesses = "procs_running";
const std::string field_pidUID = "Uid:";
const std::string field_pidVMSize = "VmSize";
const std::string field_cpu = "cpu";
//Accessed field numbers in files
constexpr int fieldnum_utime = 14;
constexpr int fieldnum_stime = 15;
constexpr int fieldnum_username = 22;

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_,
  CPUStatesNum_
};

std::vector<std::string> CpuUtilization();
std::vector<std::string> CpuUtilization(int n);
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

//helper functions
int GetValueForField(const std::string& filePath, const std::string& fieldName);
std::vector<std::string> GetValuesForField(const std::string& filePath, const std::string& fieldName);
long int GetValueAtRow(const std::string& filePath, int rowNum);

};  // namespace LinuxParser

#endif