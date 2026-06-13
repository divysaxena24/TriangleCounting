#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

class Graph
{
private:
    size_t V;

public:
    std::vector<std::vector<int>> adj;

    Graph(size_t n);

    void addEdge(int u, int v);

    size_t getVertices() const;

    std::vector<int> bfsLevels(int src);

    long long countTriangles(int r);
};

Graph createGraphFromFile(const std::string& filename);

#endif