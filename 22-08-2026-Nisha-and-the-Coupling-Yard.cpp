#include <stdio.h>
#include <stdlib.h>

void minImbalance(int n, int m, long long weights[], long long* min_imbalance, int* start_index) {
    // Allocate arrays to act as double-ended queues for indices
    int* max_dq = (int*)malloc(sizeof(int) * n);
    int* min_dq = (int*)malloc(sizeof(int) * n);
    
    int max_head = 0, max_tail = 0;
    int min_head = 0, min_tail = 0;
    
    *min_imbalance = -1; // Flag for uninitialized minimum
    *start_index = 1;
    
    for (int i = 0; i < n; ++i) {
        // Maintain monotonic decreasing order for max_dq
        while (max_tail > max_head && weights[max_dq[max_tail - 1]] <= weights[i]) {
            max_tail--;
        }
        max_dq[max_tail++] = i;
        
        // Maintain monotonic increasing order for min_dq
        while (min_tail > min_head && weights[min_dq[min_tail - 1]] >= weights[i]) {
            min_tail--;
        }
        min_dq[min_tail++] = i;
        
        // Remove elements outside the current sliding window of size m
        while (max_dq[max_head] <= i - m) {
            max_head++;
        }
        while (min_dq[min_head] <= i - m) {
            min_head++;
        }
        
        // A full window is formed starting from index (i - m + 1)
        if (i >= m - 1) {
            long long current_imbalance = weights[max_dq[max_head]] - weights[min_dq[min_head]];
            int current_start = i - m + 2; // 1-based indexing
            
            if (*min_imbalance == -1 || current_imbalance < *min_imbalance) {
                *min_imbalance = current_imbalance;
                *start_index = current_start;
            }
        }
    }
    
    free(max_dq);
    free(min_dq);
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    
    long long* weights = (long long*)malloc(sizeof(long long) * n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &weights[i]);
    }
    
    long long min_imbalance;
    int start_index;
    
    minImbalance(n, m, weights, &min_imbalance, &start_index);
    
    printf("%lld %d\n", min_imbalance, start_index);
    
    free(weights);
    return 0;
}
