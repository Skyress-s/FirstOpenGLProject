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
        // DijNode noewNode* = new DijNode(point);

        DijNode* newNode = new DijNode(point);
        
        dijNodes.push_back(newNode);
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

    startNode->distance = 0;

    // setting heuristics
    for (int i = 0; i < nodes; ++i) {
        dijNodes[i]->heuristic = GetHuerstic(dijNodes[i]);
    }
    

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

    DijNode* current = startNode;

    bool bFoundEnd = false;
    
    int x = 0;
    while (x < nodes * 2) {
        x++;
        current->bVisited = true;

        for (int i = 0; i < current->edges.size(); ++i) {
            DijNode* other = current->edges[i]->GetOther(current);
            // bail
            if (other->bVisited)
                continue;

            float distanceToNode = current->distance + current->edges[i]->distance;
            float g = current->distance + current->edges[i]->distance;

            std::cout << "distance to node!" << distanceToNode << std::endl;
            // decrease if lower
            if (distanceToNode < other->distance) {
                other->distance = distanceToNode;
                other->from = current;
            }

            if (other == endNode) {
                endNode->from = current;
                bFoundEnd = true;
                break;
            }
        }

        if (bFoundEnd) {
            break;
        }
        
        // has gone tgrough all edges
        Pop();
        
        // sets shortest to new current
        // dijNodes[0]->from = current;
        current = dijNodes[0];
    }

    // make sure to pop end node
    Pop();

    DebugDijNodes();

    // return;
    // display path
    int y = 0;
    DijNode* ja = endNode;
    while (y < nodes * 2) {
        y++;
        std::cout << " FROM : " << ja->from << std::endl;
        ja->color = glm::vec3(1,1,0);
        if (ja->from != nullptr) {
            DijEdge* edge = ja->GetEdge(ja->from);
            if (edge != nullptr) {
                
                edge->color = glm::vec3(0,1,0);
            }
            ja = ja->from;
        }
        else {
            break;
        }

    }

    startNode->color = glm::vec3(0,1,0);
    endNode->color = glm::vec3(1,0,0);
    
}

float Dij::DijsktraHandler::GetHuerstic(Dij::DijNode* node) {
    return glm::distance(node->position, endNode->position);
}

void Dij::DijsktraHandler::DebugDijNodes() {
    std::cout << "Nodes" << std::endl;
    for (int i = 0; i < dijNodes.size(); ++i) {
        std::cout << i << " | " << dijNodes[i]->distance << std::endl;
    }
    std::cout << "|" << std::endl;
    std::cout << "FinishedNodes" << std::endl;
    for (int i = 0; i < finishedDijNodes.size(); ++i) {
        std::cout << i << " | " << finishedDijNodes[i]->distance << std::endl;
    }
}

void Dij::DijsktraHandler::Pop() {
    Prioritize();
    // travel to next node
    pop_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
    finishedDijNodes.push_back(dijNodes.back());
    dijNodes.pop_back();
    Prioritize();
}

void Dij::DijsktraHandler::Prioritize() {
    make_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
    sort_heap(dijNodes.begin(), dijNodes.end(), DijNodeComparator());
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
