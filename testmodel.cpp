#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class DSU {
private:
    vector<int> parent, size;

public:
    DSU(int n) : parent(n+1), size(n+1, 1) {
        for (int i = 1; i <= n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        
        if (rx == ry) return;
        
        if (size[rx] < size[ry]) {
            parent[rx] = ry;
            size[ry] += size[rx];
        } else {
            parent[ry] = rx;
            size[rx] += size[ry];
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    string s;
    cin >> s;
    
    // Key insight: Precompute all edges that will exist at each timestep
    vector<vector<pair<int, int>>> timestep_edges(n+1);
    
    // Read original edges
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        edges.push_back({u, v});
    }
    
    // For each node, store its neighbors in the original graph
    vector<vector<int>> adj(n+1);
    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Precompute all edges at each timestep
    // Start with all edges and simulate removals
    vector<bool> removed(n+1, false);
    for (int t = 1; t <= n; t++) {
        // Store original edges between nodes that still exist
        for (auto [u, v] : edges) {
            if (!removed[u] && !removed[v]) {
                timestep_edges[t-1].push_back({u, v});
            }
        }
        
        // If s[t-1] = '1', record the edges to add between its neighbors
        if (s[t-1] == '1') {
            vector<int> neighbors;
            for (int neighbor : adj[t]) {
                if (!removed[neighbor]) {
                    neighbors.push_back(neighbor);
                }
            }
            
            // Add these edges to all future timesteps
            for (size_t i = 0; i < neighbors.size(); i++) {
                for (size_t j = i+1; j < neighbors.size(); j++) {
                    int u = neighbors[i];
                    int v = neighbors[j];
                    if (u > v) swap(u, v);
                    for (int future_t = t; future_t <= n; future_t++) {
                        timestep_edges[future_t-1].push_back({u, v});
                    }
                }
            }
        }
        
        // Mark node t as removed
        removed[t] = true;
    }
    
    // Compute the answers
    vector<long long> result(n);
    for (int t = 1; t <= n; t++) {
        DSU dsu(n);
        
        // Process edges for this timestep
        for (auto [u, v] : timestep_edges[t-1]) {
            dsu.unite(u, v);
        }
        
        // Count connected pairs
        unordered_map<int, int> component_size;
        for (int i = t; i <= n; i++) {
            component_size[dsu.find(i)]++;
        }
        
        long long pairs = 0;
        for (auto& [root, size] : component_size) {
            pairs += (1LL * size * (size - 1)) / 2;
        }
        
        result[t-1] = pairs;
    }
    
    // Output results
    for (int i = 0; i < n; i++) {
        cout << result[i] << endl;
    }
    
    return 0;
}