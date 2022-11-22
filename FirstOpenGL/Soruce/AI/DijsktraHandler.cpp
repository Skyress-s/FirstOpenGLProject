#include "DijsktraHandler.h"

#include <functional>
#include <iostream>
#include <queue>
#include "../Line.h"
#include "DijEdge.h"

Dij::DijsktraHandler::DijsktraHandler(int nodes, float sphereSize) {
    this->nodes = nodes;
    this->sphereSize = sphereSize;

    // generate nodes with positions
    for (int i = 0; i < nodes; ++i) {

        glm::vec3 point = myRandom::GetRandomInsideUnitSphere() * 3.5f;

        dijNodes.push_back(new Dij::DijNode(point, myRandom::GetRandomFloat(0, 100)));
    }

    // setting up edges
    for (int i = 0; i < nodes; ++i) {
        std::vector<DijNode*> closests = FindXClosestNodes(3, dijNodes[i]);
        for (int j = 0; j < closests.size(); ++j) {
            DijEdge* edge = new DijEdge(dijNodes[i], closests[j]);

            if (!edge->first->HasEdge(edge)) {
                edge->first->edges.push_back(edge);

            }
            if (!edge->second->HasEdge(edge)) {
                edge->second->edges.push_back(edge);
            }

            dijEdges.push_back(edge);
        }

    }

    int startNodeIndex = myRandom::GetRandomInt(0, nodes);
    int endNodeIndex = myRandom::GetRandomInt(0, nodes);

    dijNodes[startNodeIndex]->color = glm::vec3(0, 1, 0);
    dijNodes[endNodeIndex]->color = glm::vec3(1, 0, 0);
    
    startNode = dijNodes[startNodeIndex];
    endNode = dijNodes[endNodeIndex];

    // making into priority list   
    make_heap(dijNodes.begin(), dijNodes.end(), Dij::DijNodeComparator());
    sort_heap(dijNodes.begin(), dijNodes.end(), Dij::DijNodeComparator());
}

void Dij::DijsktraHandler::DrawEdges(glm::mat4 MVP) {
    for (int i = 0; i < dijEdges.size(); ++i) {
        dijEdges[i]->Draw(MVP);
    }

}

void Dij::DijsktraHandler::FindShortestPath() {
    // setup start
    // dijNodes[startNode]->distance = 0;
    // make_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
    // sort_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
    int x = 0;
    // while (x < 1) {
    x++;
    DijNode* current = startNode;
    current->bVisited = true;

    for (int i = 0; i < current->edges.size(); ++i) {
        DijNode* other = current->edges[i]->GetOther(current);
        // bail
        if (other->bVisited)
        continue;

        float distanceToNode = current->distance + current->edges[i]->distance;

        if (distanceToNode < other->distance) {
            other->distance = distanceToNode;
        }



        make_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
        sort_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());

        // travel to next node
        pop_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
        finishedDijNodes.push_back(dijNodes.back());
        dijNodes.pop_back();

        make_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
        sort_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
    }

    DebugDijNodes();
}

void Dij::DijsktraHandler::DebugDijNodes() {
    for (int i = 0; i < dijNodes.size(); ++i) {
        std::cout << i << " | " << dijNodes[i]->ToString() << std::endl;
    }
}

std::vector<Dij::DijNode*> Dij::DijsktraHandler::FindXClosestNodes(int x, DijNode* current) {
    std::vector<DijNode*> distanceNodes{};

    std::function<bool(DijTuple, DijTuple)> comp = [](const DijTuple s1, const DijTuple s2) {
        return s1.first > s2.first;
    };

    std::priority_queue<DijTuple, std::vector<DijTuple>, decltype(comp)> distanceQueue(comp);
    for (int i = 0; i < nodes; ++i) {
        if (current == dijNodes[i])
            continue;
        DijTuple dist{};
        dist.first = glm::distance(current->position, dijNodes[i]->position);
        dist.second = i;
        distanceQueue.push(dist);
    }

    std::vector<DijNode*> retVal{};
    for (int i = 0; i < x; ++i) {
        retVal.push_back(dijNodes[distanceQueue.top().second]);
        distanceQueue.pop();
    }

    return retVal;
}
