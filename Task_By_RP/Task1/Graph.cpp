#include "Graph.h"

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

long long Graph::countTriangles(int r)
{
    auto pack = [](int a, int b)
    {
        if (a > b)
            swap(a, b);

        return ((long long)a << 32) |
               (unsigned int)b;
    };

    //--------------------------------------------------
    // Build edge list and Edge IDs Φ
    //--------------------------------------------------

    vector<pair<int,int>> edges;

    unordered_map<long long,int> edgeId;

    int id = 0;

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            if (u < v)
            {
                edges.push_back({u,v});
                edgeId[pack(u,v)] = id++;
            }
        }
    }

    //--------------------------------------------------
    // Cover Set Generation
    //--------------------------------------------------

    vector<int> horizontalCount(id,0);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<>
        dist(0,V-1);

    for (int iter = 0; iter < r; iter++)
    {
        int src = dist(gen);

        vector<int> level =
            bfsLevels(src);

        for (int u = 0; u < V; u++)
        {
            for (int v : adj[u])
            {
                if (u < v &&
                    level[u] != -1 &&
                    level[v] != -1 &&
                    level[u] == level[v])
                {
                    horizontalCount[
                        edgeId[pack(u,v)]
                    ]++;
                }
            }
        }
    }

    int threshold =
        (int)ceil(r / 3.0);

    unordered_set<long long> coverSet;

    for (int i = 0; i < (int)edges.size(); i++)
    {
        if (horizontalCount[i] >= threshold)
        {
            coverSet.insert(
                pack(edges[i].first,
                     edges[i].second)
            );
        }
    }

    cout << "\nCover Set Size = "
         << coverSet.size()
         << endl;

    //--------------------------------------------------
    // Fast adjacency lookup
    //--------------------------------------------------

    vector<unordered_set<int>>
        adjSet(V);

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            adjSet[u].insert(v);
        }
    }

    //--------------------------------------------------
    // Algorithm:
    // Count triangles using Cover Set + Φ
    //--------------------------------------------------

    long long T = 0;

    for (auto &edge : edges)
    {
        int u = edge.first;
        int v = edge.second;

        long long uv =
            pack(u,v);

        if (!coverSet.count(uv))
            continue;

        int phi_uv =
            edgeId[uv];

        //--------------------------------------------------
        // Find N(u) ∩ N(v)
        //--------------------------------------------------

        if (adj[u].size() >
            adj[v].size())
        {
            swap(u,v);
        }

        for (int w : adj[u])
        {
            if (!adjSet[v].count(w))
                continue;

            bool isMaximum = true;

            long long uw =
                pack(u,w);

            long long vw =
                pack(v,w);

            //--------------------------------------------------
            // If (u,w) ∈ CS and
            // Φ(u,w) > Φ(u,v)
            //--------------------------------------------------

            auto it1 =
                edgeId.find(uw);

            if (it1 != edgeId.end())
            {
                if (coverSet.count(uw) &&
                    it1->second > phi_uv)
                {
                    isMaximum = false;
                }
            }

            //--------------------------------------------------
            // If (v,w) ∈ CS and
            // Φ(v,w) > Φ(u,v)
            //--------------------------------------------------

            auto it2 =
                edgeId.find(vw);

            if (it2 != edgeId.end())
            {
                if (coverSet.count(vw) &&
                    it2->second > phi_uv)
                {
                    isMaximum = false;
                }
            }

            if (isMaximum)
            {
                T++;
            }
        }
    }

    return T;
}