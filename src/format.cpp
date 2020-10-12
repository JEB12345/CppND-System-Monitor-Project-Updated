#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours, min, minSec, sec;
    hours = seconds / 3600;
    minSec = seconds % 3600;
    min = minSec / 60;
    sec = minSec % 60;
    string output = to_string(hours) + ":" + to_string(min) + ":" + to_string(sec);
    return output; 
}