#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n;
    vector<int> parent, sz;
    long long pairs; // sum of binomial(sz[comp], 2) across all components

    DSU(int n) : n(n), parent(n+1), sz(n+1, 0), pairs(0LL) {
        for(int i=0; i<=n; i++){
            parent[i] = i;
            // sz[i] = 0 => not "activated" yet
        }
    }

    // find with path compression
    int findp(int v){
        if(parent[v] == v) return v;
        return parent[v] = findp(parent[v]);
    }

    // Make a new set containing just node v (size=1)
    void make_set(int v){
        // We assume we call this once for each node
        // that becomes "active".
        parent[v] = v;
        sz[v] = 1;
    }

    // merges the sets of a and b (if distinct)
    void unite(int a, int b){
        a = findp(a);
        b = findp(b);
        if(a == b) return;
        // union by size
        if(sz[a] < sz[b]) std::swap(a,b);
        // now a is bigger
        parent[b] = a;
        // merging sets => add cross pairs = sz[a] * sz[b]
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
    cin >> s;  // s[i] in {'0','1'}, i from 0..N-1, corresponds to node i+1

    // neighborsPlus[i] = all j>i s.t. (i,j) is an original edge
    vector<vector<int>> neighborsPlus(N+1);
    for(int i=0; i<M; i++){
        int u,v;
        cin >> u >> v;
        if(u < v){
            neighborsPlus[u].push_back(v);
        } else if(v < u){
            neighborsPlus[v].push_back(u);
        }
    }

    // LN(k) = smallest neighbor in neighborsPlus[k], if s[k-1]=='1'
    // else LN(k)=-1
    vector<int> LN(N+1, -1);
    for(int k=1; k<=N; k++){
        if(s[k-1] == '1'){
            int mn = INT_MAX;
            for(int nxt : neighborsPlus[k]){
                mn = min(mn, nxt);
            }
            if(mn != INT_MAX){
                LN[k] = mn;
            }
        }
    }

    // lowestInv[x] = list of all k s.t. LN(k)=x
    vector<vector<int>> lowestInv(N+1);
    for(int k=1; k<=N; k++){
        if(LN[k] != -1){
            lowestInv[LN[k]].push_back(k);
        }
    }

    DSU dsu(N);
    vector<bool> present(N+1, false);

    // We'll store the result for each i
    vector<long long> res(N+1, 0LL);

    // Process from i=N down to i=1
    for(int i=N; i>=1; i--){
        // Activate i in DSU
        dsu.make_set(i);
        present[i] = true;

        // Union i with all already-present neighbors from the original adjacency
        for(int j : neighborsPlus[i]){
            if(present[j]){
                dsu.unite(i, j);
            }
        }

        // If i is LN(k) for some k, that means we should "clique-ify" neighbors^+(k)
        // i.e. unify all present neighbors of k. 
        // That simulates the edges that node k (with s_k=1) introduced upon removal.
        if(!lowestInv[i].empty()){
            for(int k : lowestInv[i]){
                // gather all present neighbors of k in neighborsPlus[k]
                vector<int> nbrs;
                nbrs.reserve(neighborsPlus[k].size());
                for(int x : neighborsPlus[k]){
                    if(present[x]){
                        nbrs.push_back(x);
                    }
                }
                // unify them in DSU
                if(!nbrs.empty()){
                    int leader = nbrs[0];
                    for(int idx = 1; idx < (int)nbrs.size(); idx++){
                        dsu.unite(leader, nbrs[idx]);
                    }
                }
            }
        }

        // number of reachable pairs after adding i
        res[i] = dsu.pairs;
    }

    // Output results in forward order
    // res[i] = #pairs just before removing node i
    for(int i=1; i<=N; i++){
        cout << res[i] << "\n";
    }

    return 0;
}
