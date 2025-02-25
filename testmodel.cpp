#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_set>

using namespace std;

// Count the number of pairs of nodes that can reach each other
long long countPairs(const vector<unordered_set<int>>& graph, const unordered_set<int>& removed_nodes) {
    vector<bool> visited(graph.size(), false);
    long long total_pairs = 0;
    
    for (int i = 1; i < graph.size(); i++) {
        if (!removed_nodes.count(i) && !visited[i]) {
            int component_size = 0;
            queue<int> q;
            q.push(i);
            visited[i] = true;
            
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                component_size++;
                
                for (int neighbor : graph[node]) {
                    if (!removed_nodes.count(neighbor) && !visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
            
            // For a component of size k, there are k*(k-1)/2 pairs
            total_pairs += 1LL * component_size * (component_size - 1) / 2;
        }
    }
    
    return total_pairs;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    string s;
    cin >> s;
    
    // Use adjacency list for efficient edge operations
    vector<unordered_set<int>> graph(N + 1);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].insert(v);
        graph[v].insert(u);
    }
    
    unordered_set<int> removed_nodes;
    vector<long long> result(N);
    
    for (int t = 1; t <= N; t++) {
        // Count pairs before removing node t
        result[t - 1] = countPairs(graph, removed_nodes);
        
        // If s[t-1] = '1', add edges between all pairs of neighbors
        if (s[t - 1] == '1') {
            vector<int> neighbors;
            for (int neighbor : graph[t]) {
                if (!removed_nodes.count(neighbor)) {
                    neighbors.push_back(neighbor);
                }
            }
            
            // Connect all neighbors with each other
            for (int i = 0; i < neighbors.size(); i++) {
                for (int j = i + 1; j < neighbors.size(); j++) {
                    graph[neighbors[i]].insert(neighbors[j]);
                    graph[neighbors[j]].insert(neighbors[i]);
                }
            }
        }
        
        // Remove node t from the graph
        for (int neighbor : graph[t]) {
            graph[neighbor].erase(t);
        }
        graph[t].clear();
        removed_nodes.insert(t);
    }
    
    // Output results
    for (int t = 0; t < N; t++) {
        cout << result[t] << endl;
    }
    
    return 0;
}