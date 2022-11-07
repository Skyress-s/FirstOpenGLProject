#pragma once
#include <random>
static std::mt19937_64 gen64(std::random_device{}());
static std::default_random_engine e;

namespace myRandom {
    float GetRandomFloat(float min, float max) {
        std::uniform_real_distribution<> dis(min, max); // rage 0 - 1
        return dis(e);
    }
}
