#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n;
vector<int> a;
vector<vector<int>> adj;
vector<int> ans;

// Store nodes along current DFS path (1-indexed depth)
vector<int> path_nodes;

// Segment tree to store maximum strength in depth range [1, N]
vector<int> seg_tree;

void update(int node, int start, int end, int idx, int val) {
    if (start == end) {
        seg_tree[node] = val;
        return;
    }
    int mid = start + (end - start) / 2;
    if (idx <= mid)
        update(2 * node, start, mid, idx, val);
    else
        update(2 * node + 1, mid + 1, end, idx, val);
    seg_tree[node] = max(seg_tree[2 * node], seg_tree[2 * node + 1]);
}

// Find the largest depth index in [1, max_depth] with strength > target_val
int query(int node, int start, int end, int l, int r, int target_val) {
    // If range out of bounds or max value in segment <= target_val, no answer here
    if (l > end || r < start || seg_tree[node] <= target_val) {
        return -1;
    }
    // Leaf node reached
    if (start == end) {
        return start;
    }
    
    int mid = start + (end - start) / 2;
    
    // Always check the RIGHT child first to find the CLOSEST (deepest) ancestor
    int right_res = query(2 * node + 1, mid + 1, end, l, r, target_val);
    if (right_res != -1) return right_res;
    
    return query(2 * node, start, mid, l, r, target_val);
}

void dfs(int u, int p, int depth) {
    // 1. Query closest ancestor with strength > a[u]
    if (depth == 1) {
        ans[u] = -1; // Root has no ancestors
    } else {
        int ancestor_depth = query(1, 1, n, 1, depth - 1, a[u]);
        if (ancestor_depth == -1) {
            ans[u] = -1;
        } else {
            ans[u] = path_nodes[ancestor_depth];
        }
    }

    // 2. Add current node to path and Segment Tree
    path_nodes[depth] = u;
    update(1, 1, n, depth, a[u]);

    // 3. Recurse to children
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, depth + 1);
        }
    }

    // 4. Backtrack: reset current depth in Segment Tree
    update(1, 1, n, depth, 0);
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n)) return 0;

    adj.resize(n + 1);
    a.resize(n + 1);
    ans.resize(n + 1);
    path_nodes.resize(n + 1);
    seg_tree.assign(4 * n + 1, 0);

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    // DFS starting from root (node 1) at depth 1
    dfs(1, 0, 1);

    // Output results
    for (int i = 1; i <= n; ++i) {
        cout << ans[i] << (i == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}
