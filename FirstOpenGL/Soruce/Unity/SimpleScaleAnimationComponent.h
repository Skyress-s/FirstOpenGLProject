#pragma once
#include "Component.h"

class SimpleScaleAnimationComponent : public Component {
public:
    float _startTime;
    bool _bActiveAnim = false;
    void PlayAnimation();

    
    void Start(GameObject* test) override;
    void Update(GameObject* test) override;
};
