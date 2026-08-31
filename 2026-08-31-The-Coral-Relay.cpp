#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>

struct DSU {
    std::vector<int> parent;
    std::vector<int> rank;
    int components;

    DSU(int n) : parent(n + 1), rank(n + 1, 0), components(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]); // Path compression
    }

    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j])
                std::swap(root_i, root_j);
            parent[root_j] = root_i;
            if (rank[root_i] == rank[root_j])
                rank[root_i]++;
            components--;
            return true;
        }
        return false;
    }
};

long long min_relay_cost(int n, int m, std::vector<std::tuple<int, int, int>> &edges) {
    // If there is only 1 pod, 0 cost is needed
    if (n <= 1) return 0;

    // Sort edges by weight w in ascending order
    // Tuple structure: (u, v, w) -> sort primarily by w
    std::sort(edges.begin(), edges.end(), [](const auto &a, const auto &b) {
        return std::get<2>(a) < std::get<2>(b);
    });

    DSU dsu(n);
    long long total_cost = 0;
    int edges_used = 0;

    for (const auto &[u, v, w] : edges) {
        if (dsu.unite(u, v)) {
            total_cost += w;
            edges_used++;
            if (edges_used == n - 1) {
                break;
            }
        }
    }

    // If we connected all n pods using n - 1 edges
    if (edges_used == n - 1) {
        return total_cost;
    }

    return -1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    if (!(std::cin >> n >> m)) return 0;

    std::vector<std::tuple<int, int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        edges[i] = std::make_tuple(u, v, w);
    }

    long long result = min_relay_cost(n, m, edges);
    std::cout << result << "\n";

    return 0;
}
