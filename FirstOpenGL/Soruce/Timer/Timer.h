#pragma once
#include <chrono>
#include <iostream>

class Timer {
public:
    Timer() {
        m_StartTimePoint = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        Stop();
    }

    long long Stop() {
        std::chrono::time_point<std::chrono::steady_clock> endTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimePoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimePoint).time_since_epoch().count();
        auto duration = end - start;

        // std::cout << duration << "us\n";
        return duration;
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
};
