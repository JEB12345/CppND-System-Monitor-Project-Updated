#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Needed to properly display numbers in default ncurses display.
string Format::pad(string s, int length, char pad) {
    s.insert(s.begin(), length - s.size(), pad);
    return s;
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hours, min, sec;
    min = seconds / 60;
    hours = min / 60;
    min = min % 60;
    sec = seconds % 60;
    string output = Format::pad(to_string(hours), 2, '0') + ":" + Format::pad(to_string(min), 2, '0') + ":" + Format::pad(to_string(sec), 2, '0');
    return output; 
}