#include "Time.h"
long long Time::_time = 0;
long long Time::_last = 0;
long long Time::_deltaTime = 0.0;
std::chrono::time_point<std::chrono::steady_clock> Time::_startTimePoint = std::chrono::high_resolution_clock::now();