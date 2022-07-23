#pragma once
#include <glm/glm/vec3.hpp>


struct Transform {
   glm::vec3 position{};
   glm::vec3 rotation{};
   glm::vec3 scale{};
   
   Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) {
      position = _position;
      rotation = _rotation;
      scale = _scale;
   }
   
};
