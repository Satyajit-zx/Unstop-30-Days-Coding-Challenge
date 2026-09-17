#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    long long B;

    if (scanf("%d %lld", &n, &B) != 2) {
        return 0;
    }

    long long *cost = (long long *)malloc((size_t)n * sizeof(long long));
    for (int i = 0; i < n; i++) {
        scanf("%lld", &cost[i]);
    }

    int max_len = 0;
    int left = 0;
    long long current_sum = 0;

    for (int right = 0; right < n; right++) {
        current_sum += cost[right];

        // Shrink window from the left until sum <= B
        while (current_sum > B && left <= right) {
            current_sum -= cost[left];
            left++;
        }

        // Calculate window length if valid
        int current_len = right - left + 1;
        if (current_len > max_len) {
            max_len = current_len;
        }
    }

    printf("%d\n", max_len);

    free(cost);
    return 0;
}
