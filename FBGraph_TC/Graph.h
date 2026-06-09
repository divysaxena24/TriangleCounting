#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_set>

class Graph
{
private:
    int V;
    std::vector<std::vector<int>> adj;
    std::vector<std::unordered_set<int>> adjSet;

public:
    Graph(int V);

    void addEdge(int u, int v);

    int getVertices();

    long long countTrianglesCoverEdge();
};

#endif