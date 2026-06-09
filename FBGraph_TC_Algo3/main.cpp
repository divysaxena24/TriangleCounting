#include "Graph.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

    // PASS 1: find max vertex id
    while (fin >> u >> v)
    {
        maxVertex = max(maxVertex, u);
        maxVertex = max(maxVertex, v);
    }

    fin.close();

    Graph g(maxVertex + 1);

    // PASS 2: load graph
    fin.open(filename);

    while (fin >> u >> v)
    {
        if (u != v)
            g.addEdge(u, v);
    }

    fin.close();

    cout << "Vertices : "
         << g.getVertices()
         << endl;

    cout << "Triangles : "
         << g.countTriangles()
         << endl;

    return 0;
}