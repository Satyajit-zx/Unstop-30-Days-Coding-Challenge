#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Lot {
    long long start;
    long long end;
    long long profit;
};

// Comparator to sort lots by their end times
bool compareLots(const Lot &a, const Lot &b) {
    return a.end < b.end;
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Lot> lots(n);
    for (int i = 0; i < n; ++i) {
        cin >> lots[i].start >> lots[i].end >> lots[i].profit;
    }

    // Sort intervals by ascending order of end time
    sort(lots.begin(), lots.end(), compareLots);

    // dp[i] stores the maximum profit considering the first i lots
    vector<long long> dp(n + 1, 0);
    vector<long long> end_times(n);
    for (int i = 0; i < n; ++i) {
        end_times[i] = lots[i].end;
    }

    for (int i = 1; i <= n; ++i) {
        // Option 1: Do not include the current lot
        long long exclude_profit = dp[i - 1];

        // Option 2: Include the current lot
        // Find the latest lot that ends <= lots[i - 1].start using binary search
        auto it = upper_bound(end_times.begin(), end_times.begin() + i - 1, lots[i - 1].start);
        int last_valid_idx = distance(end_times.begin(), it); // 1-based index in DP array

        long long include_profit = lots[i - 1].profit + dp[last_valid_idx];

        dp[i] = max(exclude_profit, include_profit);
    }

    cout << dp[n] << "\n";

    return 0;
}
