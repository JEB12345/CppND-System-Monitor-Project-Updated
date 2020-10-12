#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long totalTime = LinuxParser::ActiveJiffies(pid_);
    long starttime = UpTime();
    long uptime = LinuxParser::Jiffies();
    float seconds = ((float)uptime - (float)starttime) / _SC_CLK_TCK;
    cpuU_ = (100 * (((float)totalTime / _SC_CLK_TCK) / seconds));
    return cpuU_; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    if(cpuU_ < a.cpuU_) {
        return true;
    } 
    else {
        return false;
    }
}