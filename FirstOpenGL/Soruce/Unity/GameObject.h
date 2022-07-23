#pragma once
#include <vector>
#include <glm/glm/vec3.hpp>
#include "Component.h"
#include "enums.h"
#include "../Transfrom/Transform.h"

class GameObject {
public:
    Transform transform{glm::vec3(1.f),glm::vec3(0.f), glm::vec3(1.f)};

    void Update() {
        for (auto element : _components) {
            element->Update(this);
        }
    }

    void Start() {
        for (auto element : _components) {
            element->Start(this);
        }
    }
    
    EGridState _state = EGridState::unselected;
    
    void AddComponent(Component* component) {
        _components.push_back(component);
    }

    template<class T>
    T* GetComponent() {
        for (int i = 0; i < _components.size(); ++i) {
            T* component = dynamic_cast<T*>(_components[0]);
            if (component != nullptr) {
                return component;
            }
        }
        //if we dont have the component
        return nullptr;
    }
private:
    std::vector<Component*> _components{};
};
