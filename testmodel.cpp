#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000000007;

// We will store all "max-gap" permutations in a global vector.
// - If N is even, it has size 2.
// - If N is odd, it has size 2*N.
static vector<vector<int>> allPerms; // allPerms[k] is a permutation of [0..N-1].

// Build the 2 permutations that achieve min-gap = N/2 when N is even
// A known pattern for an even N is something like [N/2, 0, N/2+1, 1, ..., N-1, (N/2 - 1)].
static void buildMaxGapEven(int N) {
    // One systematic way is:
    //   p[2k] = (N/2 + k) mod N   (for k=0..N/2-1)
    //   p[2k+1] = k              (for k=0..N/2-1)
    // That yields one valid permutation 'A'.
    // The other valid permutation 'B' is simply the reverse of 'A'.
    vector<int> permA(N), permB(N);
    int half = N/2;
    for (int k = 0; k < half; k++) {
        permA[2*k]   = half + k; 
        permA[2*k+1] = k;
    }
    permB = permA;
    reverse(permB.begin(), permB.end());

    allPerms.push_back(permA);
    allPerms.push_back(permB);
}

// Build the 2N permutations that achieve min-gap = (N-1)/2 when N is odd
// We step around the circle with step size M=(N-1)/2 and also with step (N - M).
static void buildMaxGapOdd(int N) {
    int M = (N - 1) / 2;  // step size
    // We do 2 permutations for each start s in [0..N-1]:
    // 1) stepping by +M mod N
    // 2) stepping by -M mod N  (which is the same as (N-M) mod N)
    // Each yields min-gap >= M, and these collectively are all 2N distinct solutions.
    allPerms.reserve(2*N);  // to avoid repeated reallocation

    for(int start=0; start<N; start++){
        // Forward step: +M mod N
        {
            vector<int> perm(N);
            int cur = start;
            for(int i=0; i<N; i++){
                perm[i] = cur;
                cur = (cur + M) % N;
            }
            allPerms.push_back(std::move(perm));
        }
        // Backward step: + (N - M) mod N
        {
            vector<int> perm(N);
            int cur = start;
            int step = N - M; // same as -M mod N
            for(int i=0; i<N; i++){
                perm[i] = cur;
                cur = (cur + step) % N;
            }
            allPerms.push_back(std::move(perm));
        }
    }
}

// Check if a given permutation perm satisfies all K constraints: p_i = j.
static bool checkConstraints(const vector<int>& perm, const vector<pair<int,int>>& constraints) {
    for(const auto &c : constraints){
        int i = c.first;
        int j = c.second;
        if(perm[i] != j) return false;
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, N;
    cin >> T >> N;  
    // We assume the problem statement: "First line: T N", then T testcases all with the same N.

    // Pre-build all permutations in S (the set that achieves the maximum min-gap).
    if(N % 2 == 0) {
        // even N
        buildMaxGapEven(N);
    } else {
        // odd N
        // We get exactly 2N permutations.
        buildMaxGapOdd(N);
    }

    while(T--){
        int K; 
        cin >> K;

        // Read the constraints p_i = j
        vector<pair<int,int>> constraints(K);
        for(int i=0; i<K; i++){
            int idx, val;
            cin >> idx >> val;
            constraints[i] = {idx, val};
        }

        // Check each candidate permutation in allPerms.
        long long countValid = 0;
        for(const auto &perm : allPerms){
            if(checkConstraints(perm, constraints)) {
                countValid++;
            }
        }
        cout << (countValid % MOD) << "\n";
    }

    return 0;
}
