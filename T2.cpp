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

    int countTrianglesT1(int src) {

        vector<int> level = bfsLevels(src);

        vector<pair<int, int>> horizontalEdges;

        // Find horizontal edges
        for (int u = 0; u < V; u++) {

            for (int v : adj[u]) {

                if (u < v && level[u] == level[v]) {

                    horizontalEdges.push_back({u, v});
                }
            }
        }

        cout << "\nHorizontal Edges:\n";

        for (auto &e : horizontalEdges) {

            cout << "("
                 << e.first
                 << ","
                 << e.second
                 << ") ";
        }

        cout << "\n";

        // Fast adjacency lookup
        vector<unordered_set<int>> adjSet(V);

        for (int i = 0; i < V; i++) {

            for (int x : adj[i]) {

                adjSet[i].insert(x);
            }
        }

        int triangleCount = 0;

        cout << "\nTriangles Found:\n";

        for (auto edge : horizontalEdges) {

            int u = edge.first;
            int v = edge.second;

            for (int x : adj[u]) {

                if (adjSet[v].count(x)) {

                    // T1 counting condition
                    if ((level[u] != level[x]) ||
                        ((level[u] == level[x]) && (v < x))) {

                        triangleCount++;

                        cout << "("
                             << u << ","
                             << v << ","
                             << x << ")\n";
                    }
                }
            }
        }

        return triangleCount;
    }

    int findGoodSource(int r) {

        int minHorizontalEdges = INT_MAX;
        int goodSource = -1;

        for (int src = 0; src < r; src++) {

            vector<int> level = bfsLevels(src);

            int horizontalEdges = 0;

            for (int u = 0; u < V; u++) {

                for (int v : adj[u]) {

                    if (u < v && level[u] == level[v]) {

                        horizontalEdges++;
                    }
                }
            }

            cout << "Source "
                 << src
                 << " -> Horizontal Edges = "
                 << horizontalEdges
                 << endl;

            if (horizontalEdges < minHorizontalEdges) {

                minHorizontalEdges = horizontalEdges;
                goodSource = src;
            }
        }

        cout << "\nMinimum Horizontal Edges = "
             << minHorizontalEdges
             << endl;

        return goodSource;
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

    int r = 5;

    int goodSrc = g.findGoodSource(r);

    cout << "\nGood Source = "
         << goodSrc
         << endl;

    int triangleCount = g.countTrianglesT1(goodSrc);

    cout << "\n\nNumber of Triangles = "
         << triangleCount
         << endl;

    return 0;
}