#include <parlay/sequence.h>
#include <parlay/parallel.h>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <atomic>
#include <cmath>
#include <climits>

using namespace std;

class Graph {
private:
    size_t V;
    parlay::sequence<parlay::sequence<int>> adj;

public:
    long long lastCoverSetSize = 0;

public:
    Graph(size_t n);

    void addEdge(int u, int v);

    size_t getVertices() const;

    vector<int> bfsLevels(int src);

    long long countTriangles(int r);

    friend Graph createGraphFromFile(const string& filename);
};

Graph::Graph(size_t n)
    : V(n), adj(n)
{
}

void Graph::addEdge(int u, int v)
{
    auto nu = adj[u].size();
    auto nv = adj[v].size();

    adj[u].resize(nu + 1);
    adj[v].resize(nv + 1);

    adj[u][nu] = v;
    adj[v][nv] = u;
}

size_t Graph::getVertices() const
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

    vector<pair<int,int>> edges;

    unordered_map<long long,int> edgeId;

    int id = 0;

    for (size_t u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            if ((int)u < v)
            {
                edges.push_back({(int)u, v});
                edgeId[pack((int)u,v)] = id++;
            }
        }
    }

    cout << "Edges = "
         << edges.size()
         << endl;

    vector<int> horizontalCount(
    edges.size(), 0);
            
    int minHorizontalEdges =
        (r == 0 ? 0 : INT_MAX);

    /*
        Generate r distinct sources
    */

    mt19937 gen(42);

    vector<int> vertices(V);

    for(int i = 0; i < (int)V; i++)
    {
        vertices[i] = i;
    }

    shuffle(
        vertices.begin(),
        vertices.end(),
        gen);

    r = min(r, (int)V);

    vector<int> sources(
        vertices.begin(),
        vertices.begin() + r);

    /*
        Run BFS from each distinct source
    */

    for(int src : sources)
    {
        vector<int> level =
            bfsLevels(src);

        int currentHorizontal = 0;

        for(size_t e = 0;
            e < edges.size();
            e++)
        {
            int u = edges[e].first;
            int v = edges[e].second;

            if(level[u] != -1 &&
            level[v] != -1 &&
            level[u] == level[v])
            {
                horizontalCount[e]++;
                currentHorizontal++;
            }
        }

        minHorizontalEdges = min(minHorizontalEdges, currentHorizontal);
    }

    cout << "Distinct Sources = " << r << endl;

    cout << "Minimum Horizontal Edges = " << minHorizontalEdges << endl;

        int threshold =
            (int)ceil(r / 3.0);

        unordered_set<long long>
            coverSet;

        for (size_t i = 0;
            i < edges.size();
            i++)
        {
            if (horizontalCount[i] >= threshold)
            {
                coverSet.insert(
                    pack(edges[i].first,
                        edges[i].second));
            }
        }

    lastCoverSetSize = coverSet.size();

    cout << "Cover Set Size = "
        << lastCoverSetSize
        << endl;

    /*
        Save histogram:
        x-axis = horizontal count
        y-axis = number of edges
    */

    map<int,int> freq;

    for(int cnt : horizontalCount)
    {
        freq[cnt]++;
    }

    string fileName =
        "horizontal_data_r_" +
        to_string(r) +
        ".txt";

    ofstream hout(fileName);

    for(auto &p : freq)
    {
        hout << p.first
            << " "
            << p.second
            << "\n";
    }

    hout.close();

    vector<unordered_set<int>>
        adjSet(V);

    for (size_t u = 0;
         u < V;
         u++)
    {
        for (int v : adj[u])
        {
            adjSet[u].insert(v);
        }
    }

    atomic<long long> T(0);

    parlay::parallel_for(
        size_t(0),
        edges.size(),
        [&](size_t idx)
        {
            int u = edges[idx].first;
            int v = edges[idx].second;

            long long uv =
                pack(u,v);

            if (!coverSet.count(uv))
                return;

            int phi_uv =
                edgeId[uv];

            long long local = 0;

            int small =
                (adj[u].size()
                 < adj[v].size())
                ? u : v;

            int large =
                (small == u)
                ? v : u;

            for (int w : adj[small])
            {
                if (!adjSet[large]
                         .count(w))
                {
                    continue;
                }

                bool isMaximum = true;

                long long uw =
                    pack(u,w);

                long long vw =
                    pack(v,w);

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
                    local++;
                }
            }

            T.fetch_add(
                local,
                memory_order_relaxed);
        });

    return T.load();
}
 
Graph createGraphFromFile(const string& filename)
{
    ifstream fin(filename);

    if (!fin)
    {
        cout << "Cannot open file: "
             << filename
             << endl;
        exit(1);
    }

    string line;

    // Skip Matrix Market comments
    while (getline(fin, line))
    {
        if (!line.empty() &&
            line[0] != '%')
        {
            break;
        }
    }

    stringstream ss(line);

    int rows, cols, nnz;

    ss >> rows >> cols >> nnz;

    Graph g(max(rows, cols));

    auto pack = [](int a, int b)
    {
        if (a > b)
            swap(a, b);

        return ((long long)a << 32) |
               (unsigned int)b;
    };

    unordered_set<long long> seenEdges;

    long long duplicateEdges = 0;

    int u, v;
    double weight;

    while (fin >> u >> v >> weight)
    {
        // MTX -> 0 based indexing
        u--;
        v--;

        // Ignore self loops
        if (u == v)
            continue;

        long long key = pack(u, v);

        // Ignore duplicate edges
        if (seenEdges.count(key))
        {
            duplicateEdges++;
            continue;
        }

        seenEdges.insert(key);

        g.addEdge(u, v);
    }

    fin.close();

    cout << "Unique Edges Loaded = "
         << seenEdges.size()
         << endl;

    cout << "Duplicate Edges Skipped = "
         << duplicateEdges
         << endl;

    return g;
}