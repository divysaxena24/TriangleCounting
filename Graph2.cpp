#include "Graph.h"
#include <queue>
#include <map>
#include <random>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <tuple>

using namespace std;

Graph::Graph(int V)
{
    this->V = V;
    adj.resize(V);
}

void Graph::addEdge(int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int Graph::getVertices()
{
    return V;
}

vector<int> Graph::bfsLevels(int src)
{
    vector<int> level(V, -1);
    queue<int> q;
    level[src] = 0;
    q.push(src);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : adj[u])
        {
            if (level[v] == -1)
            {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
    return level;
}

// -------------------------------------------------------------
// Exact triangle counting using the cover‑set algorithm
// -------------------------------------------------------------
long long Graph::countTriangles()
{
    // ---------------------------------------------
    // BFS on all connected components
    // ---------------------------------------------
    vector<int> level(V, -1);

    for (int src = 0; src < V; src++)
    {
        if (level[src] != -1)
            continue;

        queue<int> q;
        level[src] = 0;
        q.push(src);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (int v : adj[u])
            {
                if (level[v] == -1)
                {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    // ---------------------------------------------
    // Fast adjacency lookup
    // ---------------------------------------------
    vector<unordered_set<int>> adjSet(V);

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            adjSet[u].insert(v);
        }
    }

    // ---------------------------------------------
    // Algorithm 3
    // ---------------------------------------------
    long long T = 0;

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            if (u >= v)
                continue;

            // Line 5
            if (level[u] == level[v])
            {
                // Line 6
                for (int w : adj[u])
                {
                    if (!adjSet[v].count(w))
                        continue;

                    // Line 7
                    if (level[u] != level[w] ||
                        (level[u] == level[w] && v < w))
                    {
                        T++;
                    }
                }
            }
        }
    }

    return T;
}
// -------------------------------------------------------------
// Read graph from file – with edge deduplication
// -------------------------------------------------------------
Graph createGraphFromFile(string filename)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "Cannot open file!\n";
        exit(1);
    }

    auto edgeKey = [](int u, int v)
    {
        if (u > v)
            swap(u, v);
        return ((long long)u << 32) | (unsigned int)v;
    };

    unordered_set<long long> uniqueEdges;

    string type;
    int u, v, w;
    int maxVertex = -1;

    long long rawArcs = 0;

    while (file >> type)
    {
        if (type == "a")
        {
            file >> u >> v >> w;
            rawArcs++;
            u--; v--;

            if (u == v)
                continue;

            uniqueEdges.insert(edgeKey(u, v));
            maxVertex = max(maxVertex, max(u, v));
        }
        else
        {
            string dummy;
            getline(file, dummy);
        }
    }

    if (maxVertex == -1)
    {
        cout << "No edges found!\n";
        exit(1);
    }

    Graph g(maxVertex + 1);

    for (auto key : uniqueEdges)
    {
        int u = (int)(key >> 32);
        int v = (int)key;
        g.addEdge(u, v);
    }

    return g;
}