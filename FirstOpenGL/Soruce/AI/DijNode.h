#pragma once

#include <string>
#include <vector>
#include <glm/glm/glm.hpp>

namespace Dij {

    class DijEdge;
    
    class DijNode {
    public:
        glm::vec3 position{};
        DijNode* from = nullptr;
        std::vector<class DijEdge*> edges;
        float heuristic;
        float distance;
        bool bVisited;
        glm::vec3 color{1};

        DijNode(glm::vec3 position);
        
        std::string ToString();

        bool HasEdge(DijEdge* edge);

        DijEdge* GetEdge(DijNode* node);

        // operator float() const {return 1.f;} // the (float)ClassInstance operator
    };

    
    

    class DijNodeComparator {
    public:
        bool operator() (const DijNode* l, const DijNode* r);
    };

}
