#include <bits/stdc++.h>
using namespace std;

class Graph {

    int V;
    vector<vector<int>> adj;

public:

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> bfsLevels(int src) {

        vector<int> level(V, -1);
        queue<int> q;

        level[src] = 0;
        q.push(src);

        while (!q.empty()) {

            int u = q.front();
            q.pop();

            for (int v : adj[u]) {

                if (level[v] == -1) {

                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }

        return level;
    }

    int countTriangles(int r) {

        vector<pair<int, int>> edges;
        map<pair<int, int>, int> edgeId;

        int id = 0;

        for (int u = 0; u < V; u++) {

            for (int v : adj[u]) {

                if (u < v) {

                    edges.push_back({u, v});
                    edgeId[{u, v}] = id++;
                }
            }
        }

        vector<int> cnt(id, 0);

        for (int src = 0; src < r; src++) {

            vector<int> level = bfsLevels(src);

            for (int u = 0; u < V; u++) {

                for (int v : adj[u]) {

                    if (u < v && level[u] == level[v]) {

                        int idx = edgeId[{u, v}];
                        cnt[idx]++;
                    }
                }
            }
        }

        cout << "\nHorizontal Edges:\n";

        vector<pair<int, int>> horizontalEdges;

        int threshold = ceil(r / 3.0);

        for (int i = 0; i < edges.size(); i++) {

            if (cnt[i] >= threshold) {

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

        for (int u = 0; u < V; u++) {

            for (int v : adj[u]) {

                adjSet[u].insert(v);
            }
        }

        set<tuple<int, int, int>> triangles;

        for (auto edge : horizontalEdges) {

            int u = edge.first;
            int v = edge.second;

            for (int x : adj[u]) {

                if (adjSet[v].count(x)) {

                    vector<int> t = {u, v, x};
                    sort(t.begin(), t.end());

                    triangles.insert({t[0], t[1], t[2]});
                }
            }
        }

        cout << "\nTriangles Found:\n";

        for (auto tri : triangles) {

            auto [a, b, c] = tri;

            cout << "("
                 << a << ", "
                 << b << ", "
                 << c << ")\n";
        }

        return triangles.size();
    }
};

int main() {

    Graph g(8);

    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 1);

    g.addEdge(2, 5);
    g.addEdge(5, 0);

    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 5);

    int r = 3;

    int t = g.countTriangles(r);

    cout << "\nNo. of Triangles = " << t << endl;

    return 0;
}