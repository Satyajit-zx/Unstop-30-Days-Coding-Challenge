#include <stdio.h>
#include <stdlib.h>

long long countArrangements(int n, long long k, const long long* sightings) {
    long long MOD = 1000000007;
    
    // dp[i] stores the number of valid arrangements for first i days
    long long* dp = (long long*)calloc(n + 1, sizeof(long long));
    // pref_dp[i] stores prefix sums of dp values modulo MOD
    long long* pref_dp = (long long*)calloc(n + 1, sizeof(long long));
    
    dp[0] = 1;
    pref_dp[0] = 1;
    
    int left = 0;
    long long current_sum = 0;
    
    for (int i = 0; i < n; i++) {
        current_sum += sightings[i];
        
        // Shrink the window from the left if the sum exceeds K
        while (current_sum > k) {
            current_sum -= sightings[left];
            left++;
        }
        
        // dp[i + 1] is the sum of dp[j] for j from left to i
        // Using prefix sums of dp array: sum(dp[left...i]) = pref_dp[i] - pref_dp[left-1]
        long long sum_dp = pref_dp[i];
        if (left > 0) {
            sum_dp = (sum_dp - pref_dp[left - 1] + MOD) % MOD;
        }
        
        dp[i + 1] = sum_dp;
        pref_dp[i + 1] = (pref_dp[i] + dp[i + 1]) % MOD;
    }
    
    long long result = dp[n];
    
    free(dp);
    free(pref_dp);
    
    return result;
}

int main() {
    int n;
    long long k;
    scanf("%d %lld", &n, &k);
    
    long long* sightings = (long long*)malloc(n * sizeof(long long));
    for (int i = 0; i < n; i++) {
        scanf("%lld", &sightings[i]);
    }
    
    long long result = countArrangements(n, k, sightings);
    printf("%lld\n", result);
    
    free(sightings);
    return 0;
}
