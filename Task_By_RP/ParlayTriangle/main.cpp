#include "Graph.h"

#include <iostream>
#include <chrono>

using namespace std;

int main()
{
    Graph g =
        createGraphFromFile(
            "facebook.txt"
        );

    cout
        << "Vertices = "
        << g.getVertices()
        << endl;

    int r;

    cout
        << "Enter r: ";

    cin >> r;

    auto start =
        chrono::high_resolution_clock::now();

    long long triangles =
        g.countTriangles(r);

    auto end =
        chrono::high_resolution_clock::now();

    cout
        << "\nTriangles = "
        << triangles
        << endl;

    cout
        << "Execution Time = "
        << chrono::duration_cast
           <chrono::milliseconds>
           (end - start)
           .count()
        << " ms\n";

    return 0;
}