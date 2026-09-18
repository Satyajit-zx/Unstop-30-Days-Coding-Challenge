#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200005

int speed[MAX_N];
int max_dq[MAX_N];
int min_dq[MAX_N];

int main() {
    int n;
    long long L;
    if (scanf("%d %lld", &n, &L) != 2) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &speed[i]);
    }

    int max_head = 0, max_tail = 0;
    int min_head = 0, min_tail = 0;

    int left = 0;
    int max_len = 0;

    for (int right = 0; right < n; right++) {
        // Maintain max_dq (monotonically decreasing)
        while (max_tail > max_head && speed[max_dq[max_tail - 1]] <= speed[right]) {
            max_tail--;
        }
        max_dq[max_tail++] = right;

        // Maintain min_dq (monotonically increasing)
        while (min_tail > min_head && speed[min_dq[min_tail - 1]] >= speed[right]) {
            min_tail--;
        }
        min_dq[min_tail++] = right;

        // Shrink the window from the left if the condition is violated
        while ((long long)speed[max_dq[max_head]] - speed[min_dq[min_head]] > L) {
            left++;
            if (max_dq[max_head] < left) {
                max_head++;
            }
            if (min_dq[min_head] < left) {
                min_head++;
            }
        }

        // Check if the current window is longer
        int current_len = right - left + 1;
        if (current_len > max_len) {
            max_len = current_len;
        }
    }

    printf("%d\n", max_len);

    return 0;
}
