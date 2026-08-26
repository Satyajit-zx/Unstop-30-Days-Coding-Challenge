#include <iostream>
#include <vector>
#include <tuple>

// Using 64-bit integers to prevent overflow during intermediate calculations
using ll = long long;

struct Edge {
    int to;
    ll weight;
};

void dfs1(int u, int p, const std::vector<std::vector<Edge>>& adj, 
          const std::vector<int>& values, std::vector<ll>& subtree_sum, std::vector<ll>& down_dist) {
    subtree_sum[u] = values[u - 1];
    down_dist[u] = 0;
    
    for (const auto& edge : adj[u]) {
        int v = edge.to;
        ll w = edge.weight;
        if (v == p) continue;
        
        dfs1(v, u, adj, values, subtree_sum, down_dist);
        subtree_sum[u] += subtree_sum[v];
        down_dist[u] += down_dist[v] + w * subtree_sum[v];
    }
}

void dfs2(int u, int p, const std::vector<std::vector<Edge>>& adj, 
          const std::vector<ll>& subtree_sum, ll total_sum, std::vector<ll>& ans) {
    for (const auto& edge : adj[u]) {
        int v = edge.to;
        ll w = edge.weight;
        if (v == p) continue;
        
        // Rerooting equation
        ans[v] = ans[u] + w * (total_sum - 2 * subtree_sum[v]);
        dfs2(v, u, adj, subtree_sum, total_sum, ans);
    }
}

void calculate_resonance(int n, const std::vector<int>& values, 
                         const std::vector<std::tuple<int, int, int>>& paths, 
                         std::vector<ll>& resonance_values) {
    std::vector<std::vector<Edge>> adj(n + 1);
    for (const auto& path : paths) {
        int u = std::get<0>(path);
        int v = std::get<1>(path);
        int w = std::get<2>(path);
        adj[u].push_back({v, (ll)w});
        adj[v].push_back({u, (ll)w});
    }

    std::vector<ll> subtree_sum(n + 1, 0);
    std::vector<ll> down_dist(n + 1, 0);
    std::vector<ll> ans(n + 1, 0);

    // Pass 1: Compute subtree sums and down-distances rooted at 1
    dfs1(1, 0, adj, values, subtree_sum, down_dist);

    // Base answer for root
    ans[1] = down_dist[1];
    ll total_sum = subtree_sum[1];

    // Pass 2: Reroot from 1 to all other nodes
    dfs2(1, 0, adj, subtree_sum, total_sum, ans);

    for (int i = 1; i <= n; ++i) {
        resonance_values[i - 1] = ans[i];
    }
}

int main() {
    // Optimize standard I/O operations for large inputs
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> values[i];
    }

    std::vector<std::tuple<int, int, int>> paths(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        paths[i] = std::make_tuple(u, v, w);
    }

    std::vector<ll> resonance_values(n);
    calculate_resonance(n, values, paths, resonance_values);

    for (int i = 0; i < n; ++i) {
        std::cout << resonance_values[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}
