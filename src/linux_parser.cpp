#include <dirent.h>
#include <unistd.h>
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
  string os, version, kernel;
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
float LinuxParser::MemoryUtilization() 
{ 
  float memTotal, memFree;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if(key == "MemTotal:") {
          memTotal = strtof (value.c_str(), NULL);
        }
        if(key == "MemFree:") {
          memFree = strtof (value.c_str(), NULL);
        }
      }
    }
  }
  return ((memTotal - memFree) / memTotal); 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return strtol(value.c_str(), NULL, 0);   
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpuJiffies = CpuUtilization();
  return (strtol(cpuJiffies[kUser_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kNice_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kSystem_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kIdle_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kIOwait_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kIRQ_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kSoftIRQ_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kSteal_].c_str(), NULL, 0)); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, temp, utime, stime, cutime, sutime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=0; i<17; i++) {
      linestream >> temp;
      switch (i) {
        case 13: 
          utime = temp;
          break;
        case 14: 
          stime = temp;
          break;
        case 15:
          cutime = temp;
          break;
        case 16:
          sutime = temp;
          break;
        default:
          break;
      }
    }
  }
  return (strtol(utime.c_str(), NULL, 0) + 
        strtol(stime.c_str(), NULL, 0) + 
        strtol(cutime.c_str(), NULL, 0) + 
        strtol(sutime.c_str(), NULL, 0)); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpuJiffies = CpuUtilization();
  return (strtol(cpuJiffies[kUser_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kNice_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kSystem_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kIRQ_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kSoftIRQ_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kSteal_].c_str(), NULL, 0));  
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpuJiffies = CpuUtilization();
  return (strtol(cpuJiffies[kIdle_].c_str(), NULL, 0) + 
        strtol(cpuJiffies[kIOwait_].c_str(), NULL, 0));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  vector<string> jiffieFields{user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  return jiffieFields; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, name, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> name >> value) {
        if(name == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, name, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> name >> value) {
        if(name == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()) {
    std::getline(stream, cmd);
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, name, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> name >> value) {
      if(name == "VmSize:") {
        return to_string(std::stoi(value) / 1000);
      }
    }
  }
  return "N/A"; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, name, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> name >> value) {
        if(name == "Uid:") {
          return value;
        }
      }
    }
  }
  return "-1"; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line, name, x, tempUid;
  char dilem = ':';
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::getline(linestream, name, dilem);
    std::getline(linestream, x, dilem);
    std::getline(linestream, tempUid, dilem);
    if(tempUid == uid) {
      return name;
    }
  }
  return "N/A"; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, starttime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=0; i<22; i++) {
      linestream >> starttime;
    }
  }
  return strtol(starttime.c_str(), NULL, 0); 
}