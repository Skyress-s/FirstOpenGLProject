#include "Random.h"

namespace myRandom {
    float GetRandomFloat(float min, float max) {
        std::uniform_real_distribution<> dis(min, max); // rage 0 - 1
        return dis(e);
    }

    int GetRandomInt(int min, int max) {
        std::uniform_int_distribution<> dis(min, max);
        return dis(e);
    }

    glm::vec3 GetRandomInsideUnitSphere() {
        float unitRadius = myRandom::GetRandomFloat(0, 1.f);
        float px = myRandom::GetRandomFloat(-1, 1);
        float py = myRandom::GetRandomFloat(-1, 1);
        float pz = myRandom::GetRandomFloat(-1, 1);

        float mag = sqrtf(px * px + py * py + pz * pz);
        px /= mag;
        py /= mag;
        pz /= mag;

        float c = std::pow(unitRadius, 1.f / 3.f);
        px *= c;
        py *= c;
        pz *= c;

        glm::vec3 point(px, py, pz);
        return point;
    }


}
