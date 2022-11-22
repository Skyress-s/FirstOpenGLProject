#pragma once

#include <vector>

#include "DijNode.h"
#include "../Random.h"



namespace Dij {
    class DijEdge;
    typedef std::pair<float, int> DijTuple;

    class DijsktraHandler {
    public:
        std::vector<Dij::DijNode*> dijNodes{};
        std::vector<Dij::DijNode*> finishedDijNodes{};
        std::vector<Dij::DijEdge*> dijEdges{};
        DijTuple test;
        // config
        int nodes{};
        float sphereSize{};
        DijNode* startNode{};
        DijNode* endNode{};

        // construct destrcts
        DijsktraHandler(int nodes, float sphereSize);
        
        void DrawEdges(glm::mat4 MVP);

        void FindShortestPath();

        void DebugDijNodes();

    private:
        
        // helper methods
        std::vector<DijNode*> FindXClosestNodes(int x, DijNode* current);

    };
}
