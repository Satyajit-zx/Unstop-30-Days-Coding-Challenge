#include <iostream>
#include <vector>
#include <deque>
#include <numeric>

long long maxBalancedGlow(int n, long long L, const std::vector<long long>& val) {
    std::deque<int> maxQ, minQ;
    int left = 0;
    long long current_sum = 0;
    long long max_total_glow = 0;

    for (int right = 0; right < n; ++right) {
        current_sum += val[right];

        // Maintain decreasing queue for maximums
        while (!maxQ.empty() && val[maxQ.back()] <= val[right]) {
            maxQ.pop_back();
        }
        maxQ.push_back(right);

        // Maintain increasing queue for minimums
        while (!minQ.empty() && val[minQ.back()] >= val[right]) {
            minQ.pop_back();
        }
        minQ.push_back(right);

        // Shrink window if max - min > L
        while (val[maxQ.front()] - val[minQ.front()] > L) {
            current_sum -= val[left];
            if (maxQ.front() == left) maxQ.pop_front();
            if (minQ.front() == left) minQ.pop_front();
            left++;
        }

        if (current_sum > max_total_glow) {
            max_total_glow = current_sum;
        }
    }

    return max_total_glow;
}

int main() {
    int n;
    long long L;
    
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> L;
    std::vector<long long> val(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> val[i];
    }
    
    long long result = maxBalancedGlow(n, L, val);
    std::cout << result << "\n";
    
    return 0;
}
