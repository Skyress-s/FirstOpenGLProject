#pragma once
#include <chrono>
#include <iostream>

class Time {

    
public:
    static void Init() {
        // _time = 0;
        // _startTimePoint = std::chrono::high_resolution_clock::now();
    }
    
    static long long GetTimeMiliseconds() {
        return _time;
    }

    static double GetTime() {
        return (double)_time / 1000.0;
    }

    static double GetDeltaTime() {
        return (double)_deltaTime / 1000.0;
    }

    
    
    static void UpdateTime() {
        auto endTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(_startTimePoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimePoint).time_since_epoch().count();
        auto duration = end - start;

        // std::cout << duration << std::endl;
        
        _time = duration;
        _deltaTime = _time - _last;
        _last = _time;
    }

private:
    static long long _time;
    static long long _last;
    static long long _deltaTime;
    static std::chrono::time_point<std::chrono::steady_clock> _startTimePoint;

};
