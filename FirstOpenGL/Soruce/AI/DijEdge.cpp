#include "DijEdge.h"
#include "../Line.h"

namespace Dij {

    DijEdge::DijEdge(DijNode* first, DijNode* second) {
    
        this->first = first;
        this->second = second;
    
        line = new Line(this->first->position, this->second->position);
        distance = glm::distance(first->position, second->position);
    
    }
    
    DijEdge::~DijEdge() {
        delete line;
    }
    
    bool DijEdge::Has(DijNode* node) {
        if (first != node && second != node)
            return false;
        return true;
    }
    
    DijNode* DijEdge::GetOther(DijNode* current) {
        if (current != first && current == second) {
            return first;
        }
        if (current != second && current == first) {
            return second;
        }
        return nullptr;
    }

    void DijEdge::Draw(glm::mat4 MVP) {
        line->setMVP(MVP);
        line->setColor(color);
        line->draw();
    }
}
