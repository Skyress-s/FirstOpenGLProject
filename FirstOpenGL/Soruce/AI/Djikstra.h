#pragma once

#include <vector>
#include <glm/glm/glm.hpp>
namespace Dij {


    class DijNode {
    public:
        glm::vec3 position;
        DijNode* from = nullptr;
        std::vector<DijNode*> connections;
        float val;


        DijNode(glm::vec3 position, float val) {
            this->position = position;
            this->val = val;
        }
    };
    

    class DijNodeComparator {
    public:
        bool operator() (const DijNode* l, const DijNode* r) {
            return l->val < r->val;
        }
    };

}
