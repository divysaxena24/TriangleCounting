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

    map<pair<int, int>, int> edgeId;

    int id = 0;

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            if (u < v)
            {
                edges.push_back({u, v});
                edgeId[{u, v}] = id++;
            }
        }
    }

    vector<int> cnt(id, 0);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dist(0, V - 1);

    for (int i = 0; i < r; i++)
    {
        int src = dist(gen);

        vector<int> level = bfsLevels(src);

        for (int u = 0; u < V; u++)
        {
            for (int v : adj[u])
            {
                if (u < v &&
                    level[u] != -1 &&
                    level[v] != -1 &&
                    level[u] == level[v])
                {
                    int idx = edgeId[{u, v}];
                    cnt[idx]++;
                }
            }
        }
    }
    

    cout << "\nHorizontal Edges:\n";

    vector<pair<int, int>> horizontalEdges;

    int threshold = ceil(r / 3.0);

    for (int i = 0; i < edges.size(); i++)
    {
        if (cnt[i] >= threshold)
        {
            horizontalEdges.push_back(edges[i]);

            cout << "("
                 << edges[i].first
                 << ", "
                 << edges[i].second
                 << ") count = "
                 << cnt[i]
                 << '\n';
        }
    }

    vector<unordered_set<int>> adjSet(V);

    for (int u = 0; u < V; u++)
    {
        for (int v : adj[u])
        {
            adjSet[u].insert(v);
        }
    }

    set<tuple<int, int, int>> triangles;

    for (auto edge : horizontalEdges)
    {
        int u = edge.first;
        int v = edge.second;

        for (int x : adj[u])
        {
            if (adjSet[v].count(x))
            {
                vector<int> t = {u, v, x};

                sort(t.begin(), t.end());

                triangles.insert({
                    t[0],
                    t[1],
                    t[2]
                });
            }
        }
    }
cout << "\nShowing first 20 triangles:\n";

int shown = 0;

for (auto tri : triangles){
    if (shown == 20)
        break;

    auto [a, b, c] = tri;

    cout << "("
         << a << ", "
         << b << ", "
         << c << ")\n";

    shown++;
}

return triangles.size();
}

Graph createGraphFromFile(string filename)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "Cannot open file!\n";
        exit(1);
    }

    vector<pair<int, int>> edges;

    string type;
    int u, v, w;
    int maxVertex = -1;

    while (file >> type)
    {
        if (type == "a")
        {
            file >> u >> v >> w;

            // Convert 1-based indexing to 0-based
            u--;
            v--;

            edges.push_back({u, v});

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

    for (auto edge : edges)
    {
        g.addEdge(edge.first, edge.second);
    }

    return g;
}