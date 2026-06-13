#ifndef GRAPH_H
#define GRAPH_H

#include <bits/stdc++.h>
using namespace std;

class Graph
{
private:
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V);

    void addEdge(int u, int v);

    int getVertices();

    vector<int> bfsLevels(int src);

    long long countTriangles(int r);
};

Graph createGraphFromFile(string filename);

#endif