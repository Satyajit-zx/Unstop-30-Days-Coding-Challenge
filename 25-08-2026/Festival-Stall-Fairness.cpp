#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

int longestFairSegment(const std::vector<long long>& prices, int n, long long d) {
    std::deque<int> min_dq; // Monotonically increasing values
    std::deque<int> max_dq; // Monotonically decreasing values
    
    int left = 0;
    int max_len = 0;

    for (int right = 0; right < n; ++right) {
        // Maintain min_dq: remove elements from the back that are >= prices[right]
        while (!min_dq.empty() && prices[min_dq.back()] >= prices[right]) {
            min_dq.pop_back();
        }
        min_dq.push_back(right);

        // Maintain max_dq: remove elements from the back that are <= prices[right]
        while (!max_dq.empty() && prices[max_dq.back()] <= prices[right]) {
            max_dq.pop_back();
        }
        max_dq.push_back(right);

        // Shrink window from the left if max - min > d
        while (!max_dq.empty() && !min_dq.empty() && 
               (prices[max_dq.front()] - prices[min_dq.front()] > d)) {
            if (max_dq.front() == left) {
                max_dq.pop_front();
            }
            if (min_dq.front() == left) {
                min_dq.pop_front();
            }
            left++;
        }

        max_len = std::max(max_len, right - left + 1);
    }

    return max_len;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    long long d;
    if (!(std::cin >> n >> d)) return 0;

    std::vector<long long> prices(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> prices[i];
    }

    int result = longestFairSegment(prices, n, d);
    std::cout << result << "\n";

    return 0;
}
