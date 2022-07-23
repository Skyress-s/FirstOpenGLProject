#include "SimpleScaleAnimationComponent.h"

#include <glm/glm/ext/scalar_constants.hpp>

#include "GameObject.h"
#include "Time.h"

void SimpleScaleAnimationComponent::PlayAnimation() {
    _bActiveAnim = true;
    _startTime = (float)Time::GetTime();
}

void SimpleScaleAnimationComponent::Start(GameObject* test) {
    Component::Start(test);
    PlayAnimation();
    test->_state = EGridState::selected;
}

void SimpleScaleAnimationComponent::Update(GameObject* test) {
    Component::Update(test);

    if (_bActiveAnim) {
        test->transform.scale = glm::vec3(sin(Time::GetTime() * 2.f * glm::pi<float>()) * 0.4f + 1.f);
        // test->transform.scale = glm::vec3(0.5f);
        // std::cout << "hey bebes : " << test->transform.scale.x << std::endl;
        
        if (Time::GetTime() > 2.f + _startTime) {
            _bActiveAnim = false;
            test->transform.scale = glm::vec3(1.f);
        }
    }
    
}
