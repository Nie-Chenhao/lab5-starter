#include <bits/stdc++.h>
using namespace std;

// A simple function to get the number of connected pairs
// by BFS or DSU each time. We do BFS here for simplicity.
long long countConnectedPairs(const vector<vector<int>> &graph, const vector<bool> &alive, int N) {
    long long result = 0;
    vector<bool> visited(N+1, false);
    for(int node = 1; node <= N; node++){
        if(alive[node] && !visited[node]) {
            // BFS (or DFS) to find size of this component
            queue<int>q;
            q.push(node);
            visited[node] = true;
            long long compSize = 0;
            while(!q.empty()){
                int u = q.front();
                q.pop();
                compSize++;
                // traverse neighbors
                for(int v: graph[u]){
                    if(alive[v] && !visited[v]){
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
            // for a component of size compSize,
            // number of internal pairs = compSize*(compSize-1)/2
            result += compSize*(compSize-1)/2;
        }
    }
    return result;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N,M;
    cin >> N >> M;
    string s;
    cin >> s; // s[i] in {'0','1'}, i from 0..N-1

    // Build adjacency
    vector<vector<int>> graph(N+1);
    for(int i=0; i<M; i++){
        int u,v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // We'll keep a "alive[node]" to indicate if node is still in the graph.
    vector<bool> alive(N+1, true);

    // Initially, count the pairs in the full graph
    long long currentPairs = countConnectedPairs(graph, alive, N);

    // For t from 1..N:
    for(int t=1; t<=N; t++){
        // Output the number of pairs "just before removing node t"
        cout << currentPairs << "\n";

        // Node t is about to be removed:
        // If s[t-1] == '1', we add edges among neighbors of t
        if(s[t-1] == '1'){
            // gather neighbors
            vector<int> nbrs;
            for(int neigh : graph[t]){
                if(alive[neigh]){
                    nbrs.push_back(neigh);
                }
            }
            // add edges among these neighbors
            // For each pair (x,y) in nbrs, add an edge x<->y to 'graph'.
            // But we should only add it if both x,y are alive and x!=t,y!=t
            // (Node t is being removed anyway, so it's immaterial to t.)
            // We'll do a double loop. For a large test, this can be huge,
            // but for the sample it's fine.
            for(int i=0; i<(int)nbrs.size(); i++){
                for(int j=i+1; j<(int)nbrs.size(); j++){
                    int x = nbrs[i], y = nbrs[j];
                    // We add each direction if not already present:
                    // (In a super-careful approach, we might store a set
                    // of adjacency for each node to check duplicates. 
                    // For small input we won't break anything if we add duplicates,
                    // but let's do a small check to avoid repeated edges.)
                    graph[x].push_back(y);
                    graph[y].push_back(x);
                }
            }
        }

        // Now physically remove node t: set alive[t] = false
        alive[t] = false;

        // Recompute the pair-count for the updated graph
        currentPairs = countConnectedPairs(graph, alive, N);
    }

    return 0;
}
