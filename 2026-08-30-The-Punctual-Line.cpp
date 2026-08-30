#include <iostream>
#include <vector>
#include <queue>

struct Edge {
    int to;
    long long first;
    long long freq;
    long long dur;
};

void earliest_arrival(int n, int m, int S, int D, const std::vector<std::vector<int>>& train_services) {
    // Build adjacency list (1-indexed)
    std::vector<std::vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u = train_services[i][0];
        int v = train_services[i][1];
        long long first = train_services[i][2];
        long long freq = train_services[i][3];
        long long dur = train_services[i][4];
        adj[u].push_back({v, first, freq, dur});
    }

    const long long INF = -1;
    std::vector<long long> dist(n + 1, INF);
    
    // Min-priority queue storing {earliest_time, station}
    std::priority_queue<std::pair<long long, int>, 
                        std::vector<std::pair<long long, int>>, 
                        std::greater<std::pair<long long, int>>> pq;

    dist[S] = 0;
    pq.push({0, S});

    while (!pq.empty()) {
        auto [current_time, u] = pq.top();
        pq.pop();

        // If we found a shorter path previously, skip
        if (current_time > dist[u]) continue;

        // If destination reached, print and return
        if (u == D) {
            std::cout << current_time << "\n";
            return;
        }

        for (const auto& edge : adj[u]) {
            long long dep_time = -1;

            if (current_time <= edge.first) {
                dep_time = edge.first;
            } else if (edge.freq > 0) {
                long long diff = current_time - edge.first;
                long long cycles = (diff + edge.freq - 1) / edge.freq;
                dep_time = edge.first + cycles * edge.freq;
            }

            // If departure is possible, relax edge to neighbor
            if (dep_time != -1) {
                long long arr_time = dep_time + edge.dur;
                if (dist[edge.to] == -1 || arr_time < dist[edge.to]) {
                    dist[edge.to] = arr_time;
                    pq.push({arr_time, edge.to});
                }
            }
        }
    }

    std::cout << -1 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m, S, D;
    if (!(std::cin >> n >> m >> S >> D)) return 0;

    std::vector<std::vector<int>> train_services(m, std::vector<int>(5));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 5; j++) {
            std::cin >> train_services[i][j];
        }
    }

    earliest_arrival(n, m, S, D, train_services);
    return 0;
}
