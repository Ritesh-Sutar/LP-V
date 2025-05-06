#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    // Parallel BFS
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS starting from node " << start << ":\n";

        while (!q.empty()) {
            int levelSize = q.size();

            #pragma omp parallel for
            for (int i = 0; i < levelSize; ++i) {
                int u;
                #pragma omp critical
                {
                    u = q.front();
                    q.pop();
                }

                cout << u << " ";

                // Explore neighbors
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        #pragma omp critical
                        {
                            if (!visited[v]) {
                                visited[v] = true;
                                q.push(v);
                            }
                        }
                    }
                }
            }
        }
        cout << "\n";
    }

    // Parallel DFS using task parallelism
    void parallelDFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (!visited[v]) {
                #pragma omp task firstprivate(v)
                parallelDFSUtil(v, visited);
            }
        }

        #pragma omp taskwait
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        cout << "Parallel DFS starting from node " << start << ":\n";

        #pragma omp parallel
        {
            #pragma omp single
            {
                parallelDFSUtil(start, visited);
            }
        }
        cout << "\n";
    }
};

int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    g.parallelBFS(0);
    g.parallelDFS(0);

    return 0;
}

// #pragma omp parallel for: Used in BFS to process nodes at the same level in parallel.

// #pragma omp task and #pragma omp single: Used in DFS to parallelize recursive calls.

// #pragma omp critical: Ensures safe access to shared structures like the queue and visited