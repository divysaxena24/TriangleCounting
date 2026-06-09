#include "Graph.h"

#include <iostream>
#include <fstream>
#include <algorithm>

int main()
{
    std::string filename = "facebook.txt";

    std::ifstream fin(filename);

    if(!fin)
    {
        std::cout << "Cannot open facebook.txt\n";
        return 1;
    }

    int u, v;
    int maxVertex = -1;

    //--------------------------------------
    // PASS 1
    //--------------------------------------

    while(fin >> u >> v)
    {
        maxVertex = std::max(maxVertex, u);
        maxVertex = std::max(maxVertex, v);
    }

    fin.close();

    Graph g(maxVertex + 1);

    //--------------------------------------
    // PASS 2
    //--------------------------------------

    fin.open(filename);

    while(fin >> u >> v)
    {
        if(u != v)
            g.addEdge(u, v);
    }

    fin.close();

    std::cout << "Vertices : "
              << g.getVertices()
              << std::endl;

    std::cout << "Triangles : "
              << g.countTrianglesCoverEdge()
              << std::endl;

    return 0;
}