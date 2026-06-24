#include "Graph.cpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

int main()
{
    Graph g =
        createGraphFromFile("Datasets/bcsstk32.mtx");

    cout << "Vertices = "
         << g.getVertices()
         << endl;

    vector<int> rValues(5);

    cout << "\nEnter 5 values of r:\n";

    for(int i = 0; i < 5; i++)
    {
        cin >> rValues[i];
    }

    ofstream fout("graph_data.txt");

    for(int r : rValues)
    {
        cout << "\nRunning r = "
             << r
             << endl;

        auto start =
            chrono::high_resolution_clock::now();

        long long triangles =
            g.countTriangles(r);

        auto end =
            chrono::high_resolution_clock::now();

        long long timeMs =
            chrono::duration_cast
            <chrono::milliseconds>
            (end - start)
            .count();

        cout << "Triangles = "
             << triangles
             << endl;

        cout << "Time = "
             << timeMs
             << " ms\n";

        fout << r
             << " "
             << g.lastCoverSetSize
             << "\n";
    }

    fout.close();

    cout << "\nGenerated:\n";
    cout << "graph_data.txt\n";
    cout << "horizontal_data_r_*.txt\n";

    return 0;
}