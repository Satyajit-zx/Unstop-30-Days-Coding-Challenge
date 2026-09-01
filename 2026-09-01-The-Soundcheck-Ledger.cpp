#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

int longestStableWindow(const std::vector<int>& arr, int n, long long limit) {
    std::deque<int> max_dq; // Monotonically decreasing
    std::deque<int> min_dq; // Monotonically increasing
    
    int left = 0;
    int max_len = 0;
    
    for (int right = 0; right < n; ++right) {
        // Maintain max_dq: elements must be strictly decreasing
        while (!max_dq.empty() && arr[max_dq.back()] <= arr[right]) {
            max_dq.pop_back();
        }
        max_dq.push_back(right);
        
        // Maintain min_dq: elements must be strictly increasing
        while (!min_dq.empty() && arr[min_dq.back()] >= arr[right]) {
            min_dq.pop_back();
        }
        min_dq.push_back(right);
        
        // Shrink window from the left if the condition is violated
        while (!max_dq.empty() && !min_dq.empty() && 
               (long long)arr[max_dq.front()] - arr[min_dq.front()] > limit) {
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
    long long limit;
    
    if (!(std::cin >> n >> limit)) return 0;
    
    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    int result = longestStableWindow(arr, n, limit);
    std::cout << result << "\n";

    return 0;
}
