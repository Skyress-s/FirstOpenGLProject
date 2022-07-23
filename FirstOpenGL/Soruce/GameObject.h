#pragma once
#include <glm/glm/vec3.hpp>

#include "Transfrom/Transform.h"

class GameObject {
public:
    Transform transform{glm::vec3(1.f),glm::vec3(1.f), glm::vec3(1.f)};
    
    virtual void Update();
    virtual void Start();
    
};
