#include "DijNode.h"

#include "../Line.h"
#include "DijEdge.h"
namespace Dij {


    DijNode::DijNode(glm::vec3 position, float heuristic) {
        this->position = position;
        this->heuristic = heuristic;
        distance = INFINITY;
        bVisited = false;
    }

    std::string DijNode::ToString() {
        // std::string retVal = std::to_string(distance) + " x:" + std::to_string(position.x) +" y: "
        // +std::to_string(position.y) +" z: "
        // +std::to_string(position.z) + " ";
        std::string retVal = std::to_string(distance) + " from:";
        return retVal;
    }

    bool DijNode::HasEdge(DijEdge* edge) {
        for (int i = 0; i < edges.size(); ++i) {
            
            if (edge->first == edges[i]->first && edge->second == edges[i]->second) {
                return true;
            }
            if (edge->first == edges[i]->second && edge->second  == edges[i]->first) {
                return true;
            }
        }
        return false;
    }

    bool DijNodeComparator::operator()(const DijNode* l, const DijNode* r) {
        return l->distance < r->distance;
    }
}
