#include "Graph.h"

#include <queue>
#include <unordered_map>
#include <algorithm>
#include <omp.h>

Graph::Graph(int V)
{
    this->V = V;

    adj.resize(V);
    adjSet.resize(V);
}

void Graph::addEdge(int u, int v)
{
    if(adjSet[u].count(v))
        return;

    adj[u].push_back(v);
    adj[v].push_back(u);

    adjSet[u].insert(v);
    adjSet[v].insert(u);
}

int Graph::getVertices()
{
    return V;
}

long long Graph::countTrianglesCoverEdge()
{
    //--------------------------------------
    // BFS LEVELS
    //--------------------------------------

    std::vector<int> level(V, -1);

    for(int src = 0; src < V; src++)
    {
        if(level[src] != -1)
            continue;

        std::queue<int> q;

        level[src] = 0;
        q.push(src);

        while(!q.empty())
        {
            int u = q.front();
            q.pop();

            for(int v : adj[u])
            {
                if(level[v] == -1)
                {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    //--------------------------------------
    // BUILD COVER SET CS
    //--------------------------------------

    std::vector<std::pair<int,int>> CS;

    std::unordered_map<long long,long long> phi;

    long long edgeId = 0;

    for(int u = 0; u < V; u++)
    {
        for(int v : adj[u])
        {
            if(u < v && level[u] == level[v])
            {
                CS.push_back({u,v});

                long long key =
                    ((long long)u << 32) |
                    (unsigned int)v;

                phi[key] = edgeId++;
            }
        }
    }

    //--------------------------------------
    // PARALLEL TRIANGLE COUNTING
    //--------------------------------------

    long long T = 0;

#pragma omp parallel for reduction(+:T) schedule(dynamic)
    for(long long i = 0; i < (long long)CS.size(); i++)
    {
        int u = CS[i].first;
        int v = CS[i].second;

        long long uvKey =
            ((long long)u << 32) |
            (unsigned int)v;

        long long uvPhi = phi[uvKey];

        long long localCount = 0;

        for(int w : adj[u])
        {
            if(!adjSet[v].count(w))
                continue;

            bool isMaximum = true;

            //----------------------------------
            // Check (u,w)
            //----------------------------------

            int a = std::min(u,w);
            int b = std::max(u,w);

            long long uwKey =
                ((long long)a << 32) |
                (unsigned int)b;

            auto it1 = phi.find(uwKey);

            if(it1 != phi.end() &&
               it1->second > uvPhi)
            {
                isMaximum = false;
            }

            //----------------------------------
            // Check (v,w)
            //----------------------------------

            a = std::min(v,w);
            b = std::max(v,w);

            long long vwKey =
                ((long long)a << 32) |
                (unsigned int)b;

            auto it2 = phi.find(vwKey);

            if(it2 != phi.end() &&
               it2->second > uvPhi)
            {
                isMaximum = false;
            }

            if(isMaximum)
                localCount++;
        }

        T += localCount;
    }

    return T;
}