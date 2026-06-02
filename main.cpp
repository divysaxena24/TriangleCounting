#include "Graph.h"
#include <chrono>
#include <fstream>

using namespace std;

int main()
{
    int V;

    cout << "Enter number of vertices: ";
    cin >> V;

    int E = 10 * V;

#ifdef _WIN32

    string command =
        "GTgraph\\R-MAT\\GTgraph-rmat.exe "
        "-n " + to_string(V) +
        " -m " + to_string(E) +
        " -o graph.txt";

#else

    string command =
        "./GTgraph/R-MAT/GTgraph-rmat "
        "-n " + to_string(V) +
        " -m " + to_string(E) +
        " -o graph.txt";

#endif

    cout << "\nGenerating graph using GTgraph...\n";

    int status = system(command.c_str());

    if (status != 0)
    {
        cout << "GTgraph generation failed!\n";
        return 1;
    }

    ifstream test("graph.txt");

    if (!test)
    {
        cout << "graph.txt was not generated!\n";
        return 1;
    }

    test.close();

    Graph g = createGraphFromFile("graph.txt");

    cout << "\nVertices = "
         << g.getVertices()
         << endl;

    int r = max(1, (int)sqrt(V));

    cout << "Random BFS Samples = "
         << r
         << endl;

    auto start =
        chrono::high_resolution_clock::now();

    int triangles =
        g.countTriangles(r);

    auto end =
        chrono::high_resolution_clock::now();

    cout << "\nTotal Triangles = "
         << triangles
         << endl;

    cout << "Execution Time = "
         << chrono::duration_cast<
                chrono::milliseconds>(
                end - start)
                .count()
         << " ms\n";

    return 0;
}