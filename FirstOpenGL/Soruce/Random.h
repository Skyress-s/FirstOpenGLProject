#pragma once

#ifndef M_RANDOM
#define M_RANDOM


#include <random>
#include <glm/glm/glm.hpp>
// static std::mt19937_64 gen64(std::random_device{}());
// static std::default_random_engine e{std::random_device{}()};
static unsigned int seed = 2;
static std::mt19937_64 gen64(seed);
static std::default_random_engine e{seed};

namespace myRandom {
    float GetRandomFloat(float min, float max); 

    int GetRandomInt(int min, int max);
    
    glm::vec3 GetRandomInsideUnitSphere(); 
}
#endif
