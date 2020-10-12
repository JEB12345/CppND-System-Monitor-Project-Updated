#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long jiffies, idleJiffies;
    jiffies = LinuxParser::Jiffies();
    idleJiffies = LinuxParser::IdleJiffies();
    return (((float)jiffies - (float)idleJiffies) / (float)jiffies); 
}