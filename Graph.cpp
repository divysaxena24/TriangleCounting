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

int Graph::countTriangles(int r)
{
    vector<pair<int, int>> edges;
    edges.reserve(256);

    auto pack = [](int a, int b) -> long long {
        return ((long long)a << 32) | (unsigned int)b;
    };

    unordered_map<long long, int> edgeId;
    edgeId.reserve(1024);

    int id = 0;
    long long msum = 0;
    for (int u = 0; u < V; ++u) msum += adj[u].size();
    edges.reserve(msum / 2 + 4);

    for (int u = 0; u < V; ++u)
    {
        for (int v : adj[u])
        {
            if (u < v)
            {
                edges.emplace_back(u, v);
                edgeId[pack(u, v)] = id++;
            }
        }
    }

    vector<int> cnt(id);
    if (id > 0) cnt.assign(id, 0);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, V - 1);

    for (int i = 0; i < r; ++i)
    {
        int src = dist(gen);
        vector<int> level = bfsLevels(src);

        for (int u = 0; u < V; ++u)
        {
            for (int v : adj[u])
            {
                if (u < v && level[u] != -1 && level[v] != -1 && level[u] == level[v])
                {
                    long long key = pack(u,v);
                    auto it = edgeId.find(key);
                    if (it != edgeId.end()) cnt[it->second]++;
                }
            }
        }
    }

    vector<pair<int,int>> horizontalEdges;
    horizontalEdges.reserve(edges.size());
    int threshold = (int)ceil(r / 3.0);
    for (int i = 0; i < (int)edges.size(); ++i)
        if (cnt[i] >= threshold) horizontalEdges.push_back(edges[i]);

    vector<unordered_set<int>> adjSet(V);
    for (int u = 0; u < V; ++u)
    {
        adjSet[u].reserve(adj[u].size());
        for (int v : adj[u]) adjSet[u].insert(v);
    }

    struct TripleHash { size_t operator()(const tuple<int,int,int>& t) const noexcept {
            auto [a,b,c] = t;
            uint64_t h = (uint64_t)a;
            h = h * 1000003u + (uint64_t)b;
            h = h * 1000003u + (uint64_t)c;
            return (size_t)h;
        }
    };

    unordered_set<tuple<int,int,int>, TripleHash> triangles;
    triangles.reserve(horizontalEdges.size() * 4 + 16);

    for (auto &edge : horizontalEdges)
    {
        int u = edge.first;
        int v = edge.second;
        if (adj[u].size() > adj[v].size()) swap(u,v);
        for (int w : adj[u])
        {
            if (!adjSet[v].count(w)) continue;
            int a = u, b = v, c = w;
            if (a > b) swap(a,b);
            if (b > c) swap(b,c);
            if (a > b) swap(a,b);
            triangles.emplace(a,b,c);
        }
    }

    return (int)triangles.size();
}

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
        if (u > v) swap(u,v);
        return ((long long)u << 32) | (unsigned int)v;
    };

    unordered_set<long long> uniqueEdges;
    string type;
    int u, v, w;
    int maxVertex = -1;

    while (file >> type)
    {
        if (type == "a")
        {
            if (!(file >> u >> v >> w)) break;
            u--; v--;
            if (u < 0 || v < 0 || u == v) continue;
            uniqueEdges.insert(edgeKey(u,v));
            maxVertex = max(maxVertex, max(u,v));
        }
        else { string dummy; getline(file,dummy); }
    }

    if (maxVertex == -1)
    {
        cout << "No edges found!\n";
        exit(1);
    }

    Graph g(maxVertex + 1);
    for (auto key : uniqueEdges)
    {
        int a = (int)(key >> 32);
        int b = (int)key;
        g.addEdge(a,b);
    }

    return g;
}
