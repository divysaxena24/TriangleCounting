# Graph Triangle Counting via Random BFS Sampling

This project implements **triangle counting in large undirected graphs** using a randomized BFS-based approximation algorithm. It generates massive synthetic graphs using the [GTgraph](https://gtgraph.github.io/) R-MAT generator and counts triangles via random sampling of BFS levels.

## Overview

Triangle counting is a fundamental problem in graph analysis with applications in social networks, web graphs, and biological networks. For large graphs, exact triangle counting is computationally expensive. This implementation uses a **sampling-based approach**:

1. A graph is generated using the **R-MAT** model (Recursive MATrix) from GTgraph — a realistic scale-free graph generator.
2. The algorithm picks `r = sqrt(V)` random source vertices and runs **BFS** from each.
3. For each BFS run, it identifies **horizontal edges** — edges whose both endpoints lie at the same BFS level from the source.
4. These horizontal edges are likely to be part of triangles. The algorithm then enumerates triangles by checking common neighbors of each horizontal edge's endpoints.
5. An edge is considered a "horizontal edge" candidate if it appears on the same BFS level in at least `ceil(r / 3)` of the random BFS runs.

## Project Structure

```
├── Graph.h              # Graph class declaration
├── Graph.cpp            # Graph implementation (BFS, triangle counting, file I/O)
├── main.cpp             # Driver: generates graph, runs triangle counting, reports time
├── GTgraph/             # External graph generators (R-MAT, random, SSCA2)
│   ├── R-MAT/           # R-MAT scale-free graph generator (Windows binary)
│   ├── random/          # Random graph generator
│   └── SSCA2/           # SSCA2 graph generator (HPCS Scalable Synthetic Compact Applications)
└── README.md            # This file
```

## How It Works

### Graph Generation

The program uses **GTgraph's R-MAT generator** to create a synthetic graph with `V` vertices and `E = 10 × V` directed edges. R-MAT produces graphs with power-law degree distributions, resembling real-world networks like social graphs or the web.

The generator is invoked with:
```
GTgraph/R-MAT/GTgraph-rmat.exe -n <V> -m <E> -o graph.txt
```

### Triangle Counting Algorithm

The `countTriangles(int r)` method in `Graph.cpp`:

1. **Build edge list** — enumerates all undirected edges with a unique ID.
2. **Random BFS sampling** — runs `r` BFS traversals from randomly chosen source vertices.
3. **Horizontal edge detection** — for each BFS, finds edges whose endpoints are at the same distance from the source (i.e., `level[u] == level[v]`). Increments a counter for each such edge.
4. **Filter promising edges** — keeps edges whose counter exceeds `ceil(r / 3)` (appeared on the same level in at least ~33% of BFS runs).
5. **Triangle enumeration** — for each filtered horizontal edge `(u, v)`, looks for a common neighbor `x` such that `(u, x)` and `(v, x)` are also edges. Inserts the sorted triangle triple into a set to avoid duplicates.
6. **Output** — prints the first 20 triangles found and returns the total count.

### Execution Flow (`main.cpp`)

1. Takes the number of vertices `V` as user input.
2. Generates the graph via GTgraph R-MAT.
3. Reads the generated file into a `Graph` object.
4. Sets `r = max(1, sqrt(V))` random BFS samples.
5. Runs `countTriangles(r)` and measures execution time.
6. Displays: vertex count, BFS sample count, total triangles found, and runtime in milliseconds.

## Prerequisites

- **C++17** compatible compiler (tested with g++ and MSVC)
- **GTgraph R-MAT binary** (pre-built Windows binary included at `GTgraph/R-MAT/GTgraph-rmat.exe`)
  - On Linux/macOS, you'll need to compile the GTgraph R-MAT generator from source (see below)

### Building GTgraph from Source (Linux/macOS)

Clone and build the GTgraph repository:

```bash
git clone https://github.com/Bader-Research/GTgraph.git
cd GTgraph
make
```

After building, copy the relevant generator binaries (e.g., `GTgraph-R-MAT`) into the `GTgraph/R-MAT/` directory (or adjust the paths in `main.cpp` accordingly).

## Building & Running

### Windows (using g++ or MSVC)

```bash
g++ -std=c++17 -O2 main.cpp Graph.cpp -o triangle_count.exe
triangle_count.exe
```

### Linux/macOS

```bash
# Build GTgraph R-MAT generator first (see Prerequisites above)
g++ -std=c++17 -O2 main.cpp Graph.cpp -o triangle_count
./triangle_count
```

### Input

The program prompts for the number of vertices:

```
Enter number of vertices: 1000
```

For a quick test, try `64` (which uses the small sample graph). For performance testing, try `1000` or `10000`.

### Output

```
Enter number of vertices: 100

Generating graph using GTgraph...

Vertices = 100
Random BFS Samples = 10

Horizontal Edges:
(2, 82) count = 5
...

Showing first 20 triangles:
(2, 5, 82)
...

Total Triangles = 42
Execution Time = 15 ms
```

## Graph File Format

The program reads graphs in a DIMACS-like format produced by GTgraph:

```
p sp <vertices> <edges>
a <u> <v> <weight>
```

Lines starting with `a` represent directed arcs from `u` to `v` with the given weight. The program treats all edges as undirected.

## Key Functions

| Function | Description |
|---|---|
| `Graph(int V)` | Constructor; initializes adjacency list with `V` vertices |
| `void addEdge(int u, int v)` | Adds an undirected edge between `u` and `v` |
| `vector<int> bfsLevels(int src)` | Runs BFS from `src`; returns a vector of distances to all vertices (-1 if unreachable) |
| `int countTriangles(int r)` | Counts triangles using `r` random BFS samples |
| `Graph createGraphFromFile(string filename)` | Reads a graph from a DIMACS/GTgraph file |

## Performance Notes

- The algorithm's complexity is roughly **O(r × (V + E) + H × deg_max)** where `r = sqrt(V)` is the number of BFS samples and `H` is the number of horizontal edge candidates.
- For very large graphs (`V > 10^6`), memory usage for the adjacency set (`adjSet`) becomes significant.
- The random sampling approach is a heuristic — it may miss triangles in graphs where few edges are "horizontal" with respect to random BFS sources.
