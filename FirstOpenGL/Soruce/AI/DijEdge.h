#pragma once

#include "DijNode.h"


class Line;

namespace Dij {

    class DijEdge {
    public:
        
    
        DijNode* first{};
        DijNode* second{};
        glm::vec3 color{1};
        float distance{};

        
        DijEdge(DijNode* first, DijNode* second);
    
        ~DijEdge();

    private:
        
        Line* line;

    public:
        /**
         * \brief Does this edge have this node as a vertex?
         * \param node checker
         * \return true if yes
         */
        bool Has(DijNode* node);
    
        /**
         * \brief The the vertex of the edge thats not this
         * \param current the node to not return
         * \return other vertex, returns null if current is not part of edge
         */
        DijNode* GetOther(DijNode* current);

        void Draw(glm::mat4);
    };
};
