#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // dist[i] stores the maximum remaining broadcast depth at town i.
    // -1 means the town is not covered.
    vector<int> dist(n + 1, -1);
    priority_queue<pair<int, int>> pq; // max-heap storing {remaining_power, town}

    for (int i = 0; i < k; ++i) {
        int t;
        long long p;
        cin >> t >> p;
        // Cap power at n as the maximum simple path length is n - 1
        int capped_p = min((long long)n, p);
        if (capped_p > dist[t]) {
            dist[t] = capped_p;
            pq.push({dist[t], t});
        }
    }

    // Process nodes starting from highest remaining power
    while (!pq.empty()) {
        auto [p, u] = pq.top();
        pq.pop();

        // If we found a strictly better power for town u previously, skip
        if (p < dist[u]) continue;
        if (p == 0) continue; // No remaining depth to propagate to neighbors

        for (int v : adj[u]) {
            if (dist[u] - 1 > dist[v]) {
                dist[v] = dist[u] - 1;
                pq.push({dist[v], v});
            }
        }
    }

    // Count all covered towns
    int covered_count = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] >= 0) {
            covered_count++;
        }
    }

    cout << covered_count << "\n";

    return 0;
}
