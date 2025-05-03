#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
public:
    int V;
    vector<vector<int>> adj;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << start << " "; // Print starting node

        #pragma omp parallel
        {
            while (true) {
                int current = -1;

                // Get the front node safely
                #pragma omp critical
                {
                    if (!q.empty()) {
                        current = q.front();
                        q.pop();
                    }
                }

                // No more nodes to process
                if (current == -1)
                    break;

                // Parallel loop over neighbors
                #pragma omp for
                for (int i = 0; i < adj[current].size(); i++) {
                    int neighbor = adj[current][i];

                    bool visitNow = false;

                    // Check and mark neighbor as visited
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            visitNow = true;
                            q.push(neighbor);
                        }
                    }

                    if (visitNow) {
                        #pragma omp critical
                        cout << neighbor << " ";
                    }
                }
            }
        }
    }
};

int main() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    cout << "Parallel BFS starting from node 0:" << endl;
    g.parallelBFS(0);
    cout << endl;

    return 0;
}
