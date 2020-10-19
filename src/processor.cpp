#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float utilization;
    long active, idle, delta_active;
    active = LinuxParser::ActiveJiffies();
    idle = LinuxParser::IdleJiffies();
    delta_active = active - prev_active_;
    utilization = (static_cast<float>(delta_active) / (delta_active + (idle - prev_idle_)));
    prev_active_ = active;
    prev_idle_ = idle;
    return utilization; 
}