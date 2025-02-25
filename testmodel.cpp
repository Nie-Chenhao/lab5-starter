#include <bits/stdc++.h>
using namespace std;

// DSU (Disjoint Set Union) with union-by-size and path compression
struct DSU {
    vector<int> parent, sz;
    long long pairs; // To track number of connected pairs

    DSU(int n) : parent(n+1), sz(n+1, 0), pairs(0) {
        for(int i=0; i<=n; i++){
            parent[i] = i;
            // sz[i] = 0 initially (we "activate" nodes later)
        }
    }

    int findp(int v) {
        if(parent[v] == v) return v;
        parent[v] = findp(parent[v]);
        return parent[v];
    }

    // Make a new set of size 1 for node v
    void make_set(int v) {
        parent[v] = v;
        sz[v] = 1;
        // single node => 0 pairs inside it, so pairs doesn't change
    }

    // Union by size, and update the 'pairs' count
    void unite(int a, int b) {
        a = findp(a);
        b = findp(b);
        if(a == b) return;
        // Merge smaller into bigger
        if(sz[a] < sz[b]) swap(a,b);
        // Now a is bigger
        parent[b] = a;
        // merging sets of sizes sz[a] and sz[b]:
        // we add sz[a]*sz[b] new pairs
        pairs += 1LL * sz[a] * sz[b];
        sz[a] += sz[b];
        sz[b] = 0;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    string s;
    cin >> s;  // length N, indexed [0..N-1] in C++

    // Adjacency list for each node 1..N
    // We'll store neighbors for each i in [1..N], but we'll also create
    // a separate "neighbors^+(i)" only for j>i for convenience.
    vector<vector<int>> adj(N+1);
    for(int i=0; i<M; i++){
        int u,v; 
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Build neighbors^+(i) = all j>i where (i,j) in original edges
    // plus we will find LN(k) for each k if s_k=1
    vector<vector<int>> neighPlus(N+1);
    for(int i=1; i<=N; i++){
        for(auto &nx : adj[i]){
            if(nx > i){
                neighPlus[i].push_back(nx);
            }
        }
    }

    // LN(k) = minimum j in neighPlus[k], if s[k]=='1'
    // otherwise LN(k)=-1
    vector<int> LN(N+1, -1);
    for(int k=1; k<=N; k++){
        if(s[k-1] == '1'){ 
            // find min in neighPlus[k]
            int mn = INT_MAX;
            for(int x : neighPlus[k]){
                mn = min(mn, x);
            }
            if(mn != INT_MAX){
                LN[k] = mn;
            }
        }
    }

    // Build "lowestInv[x]" = list of k's s.t. LN(k)=x
    vector<vector<int>> lowestInv(N+1);
    for(int k=1; k<=N; k++){
        if(LN[k] != -1){
            int x = LN[k];
            lowestInv[x].push_back(k);
        }
    }

    // DSU
    DSU dsu(N);

    // "present[i]" to mark which nodes are active in the DSU
    vector<bool> present(N+1,false);

    // We'll store answer in res[i], which is the number of connected pairs
    // for the set {i, i+1, ..., N}.
    vector<long long> res(N+1,0);

    // Process in descending order i = N..1
    for(int i=N; i>=1; i--){
        // Activate node i
        dsu.make_set(i);
        present[i] = true;

        // 1) Union i with any already-present j in original adjacency for (i,j) with j>i
        for(int j : neighPlus[i]){
            if(present[j]){
                dsu.unite(i,j);
            }
        }

        // 2) For each k in lowestInv[i], we unify all present nodes in neighPlus[k]
        //    because removing k (s_k=1) in forward direction implies
        //    we create a clique among neighbors>k.
        //    The 'i' we are adding is LN(k), so now is the correct moment
        //    to unify them (since all bigger nodes are already present).
        if(!lowestInv[i].empty()){
            for(int k : lowestInv[i]){
                // We'll gather all present nodes in neighPlus[k]
                // and unify them into a single DSU set
                int leader = -1;
                for(int x : neighPlus[k]){
                    if(present[x]){
                        if(leader == -1){
                            leader = x;
                        } else {
                            dsu.unite(leader, x);
                        }
                    }
                }
            }
        }

        // Store the current number of pairs
        res[i] = dsu.pairs;
    }

    // Output res[1..N]
    // res[i] = number of connected pairs just before removing node i
    for(int i=1; i<=N; i++){
        cout << res[i] << "\n";
    }

    return 0;
}
