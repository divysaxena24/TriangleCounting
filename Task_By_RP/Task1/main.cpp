#include "Graph.h"
#include <chrono>
#include <fstream>

using namespace std;

int main()
{
    string filename = "facebook.txt";

    ifstream fin(filename);

    if (!fin)
    {
        cout << "Cannot open file\n";
        return 1;
    }

    int u, v;
    int maxVertex = -1;

    while (fin >> u >> v)
    {
        maxVertex = max(maxVertex, u);
        maxVertex = max(maxVertex, v);
    }

    fin.close();

    Graph g(maxVertex + 1);

    fin.open(filename);

    while (fin >> u >> v)
    {
        if (u != v)
            g.addEdge(u, v);
    }

    fin.close();

    cout << "Vertices = "
         << g.getVertices()
         << endl;

    int r;

    cout << "Enter r: ";
    cin >> r;

    auto start =
        chrono::high_resolution_clock::now();

    long long triangles =
        g.countTriangles(r);

    auto end =
        chrono::high_resolution_clock::now();

    cout << "\nTriangles = "
         << triangles
         << endl;

    cout << "Time = "
         << chrono::duration_cast
            <chrono::milliseconds>
            (end - start)
            .count()
         << " ms\n";

    return 0;
}