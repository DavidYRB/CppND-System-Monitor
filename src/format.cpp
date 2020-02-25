#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) {
    int hours{0};
    int minutes{0};
    int seconds{0};
    string output;
    seconds = times % 60;
    minutes = times/60 %60;
    hours = times /3600 %60;
    output = std::to_string(hours) + ':' + std::to_string(minutes) + ':' + std::to_string(seconds);
    return output;
}