#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

using namespace std;

// Optimized DSU (Disjoint Set Union) with path compression and union by size
class DSU {
private:
    vector<int> parent, size;

public:
    DSU(int n) {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 1; i <= n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int root_x = find(x);
        int root_y = find(y);
        
        if (root_x == root_y) return;
        
        // Union by size (smaller to larger)
        if (size[root_x] < size[root_y]) {
            parent[root_x] = root_y;
            size[root_y] += size[root_x];
        } else {
            parent[root_y] = root_x;
            size[root_x] += size[root_y];
        }
    }

    // Count the number of pairs of nodes that can reach each other
    long long countPairs(const vector<bool>& active) {
        vector<int> component_size(parent.size(), 0);
        
        for (int i = 1; i < parent.size(); i++) {
            if (active[i])
                component_size[find(i)]++;
        }
        
        long long pairs = 0;
        for (int size : component_size)
            pairs += 1LL * size * (size - 1) / 2;
        
        return pairs;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    string s;
    cin >> s;
    
    // Use vector of sets for efficient edge operations
    vector<unordered_set<int>> graph(N + 1);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].insert(v);
        graph[v].insert(u);
    }
    
    // Track active nodes (not yet removed)
    vector<bool> active(N + 1, true);
    
    vector<long long> result(N);
    
    for (int t = 1; t <= N; t++) {
        // Rebuild DSU at each step - more efficient than doing BFS each time
        DSU dsu(N);
        
        // Add all current edges to DSU
        for (int u = 1; u <= N; u++) {
            if (active[u]) {
                for (int v : graph[u]) {
                    if (active[v] && u < v) { // Process each edge only once
                        dsu.unite(u, v);
                    }
                }
            }
        }
        
        // Count pairs before removing node t
        result[t - 1] = dsu.countPairs(active);
        
        // Store current neighbors of node t before removal
        vector<int> neighbors;
        for (int neighbor : graph[t]) {
            if (active[neighbor]) {
                neighbors.push_back(neighbor);
            }
        }
        
        // Remove node t and its edges
        for (int neighbor : graph[t]) {
            graph[neighbor].erase(t);
        }
        graph[t].clear();
        active[t] = false;
        
        // If s[t-1] = '1', add edges between all pairs of neighbors
        if (s[t - 1] == '1') {
            for (size_t i = 0; i < neighbors.size(); i++) {
                for (size_t j = i + 1; j < neighbors.size(); j++) {
                    int u = neighbors[i];
                    int v = neighbors[j];
                    graph[u].insert(v);
                    graph[v].insert(u);
                }
            }
        }
    }
    
    // Output results
    for (int t = 0; t < N; t++) {
        cout << result[t] << endl;
    }
    
    return 0;
}