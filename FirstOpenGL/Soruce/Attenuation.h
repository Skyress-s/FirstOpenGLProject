#pragma once
#include <vector>

enum EAttenuationDistance {E7 = 0, E13 = 1, E20 = 2, E32 = 3, E50 = 4, E65 = 5, E100 = 6, E160 = 7, E200 = 8, E325 = 9
    , E600 = 10, E3250 = 11};

struct AttenuationDistance {
    AttenuationDistance(float _constant, float _linear, float _quadratic)  {
        constant =_constant;
        linear = _linear;
        quadratic = _quadratic;
    }
    float constant;
    float linear;
    float quadratic;
};

class Attenuation {
public:
    static void setDistance(EAttenuationDistance distance) {
        current = attenuations[distance];
    }
    static AttenuationDistance current ;

    
    const static AttenuationDistance attenuations[12]; 

};
AttenuationDistance Attenuation::current = AttenuationDistance(1.0f,0.045f,0.0075f); 
const AttenuationDistance Attenuation::attenuations[12] = {
                        AttenuationDistance(1.0f,0.7f,1.8f), 
                        AttenuationDistance(1.0f,0.35f,0.44f), 
                        AttenuationDistance(1.0f,0.22f,0.2f), 
                        AttenuationDistance(1.0f,0.14f,0.7f), 
                        AttenuationDistance(1.0f,0.09f,0.032f), 
                        AttenuationDistance(1.0f,0.07f,0.017f), 
                        AttenuationDistance(1.0f,0.045f,0.0075f), 
                        AttenuationDistance(1.0f,0.027f,0.0028f), 
                        AttenuationDistance(1.0f,0.022f,0.0019f), 
                        AttenuationDistance(1.0f,0.014f,0.0007f), 
                        AttenuationDistance(1.0f,0.007f,0.0002f), 
                        AttenuationDistance(1.0f,0.0014f,0.000007f)
                       }; 
