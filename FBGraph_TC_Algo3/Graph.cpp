#include "Graph.h"
#include <queue>
#include <unordered_set>

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

long long Graph::countTriangles()
{
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

    vector<unordered_set<int>> adjSet(V);

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
            adjSet[u].insert(v);
    }

    long long T = 0;

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            if (u >= v)
                continue;

            if (level[u] == level[v])
            {
                for (int w : adj[u])
                {
                    if (!adjSet[v].count(w))
                        continue;

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