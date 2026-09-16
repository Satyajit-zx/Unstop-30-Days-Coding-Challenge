#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    int *readings = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &readings[i]);
    }

    // Allocate array for the deque (stores indices)
    int *deque = (int *)malloc(sizeof(int) * n);
    int head = 0;
    int tail = 0;

    for (int i = 0; i < n; i++) {
        // 1. Remove indices outside the current cycle window [i - k + 1, i]
        while (head < tail && deque[head] <= i - k) {
            head++;
        }

        // 2. Remove indices with smaller or equal values from the back
        while (head < tail && readings[deque[tail - 1]] <= readings[i]) {
            tail--;
        }

        // 3. Insert current element's index
        deque[tail++] = i;

        // 4. Output maximum reading for the current cycle
        if (i >= k - 1) {
            if (i > k - 1) {
                putchar(' ');
            }
            printf("%d", readings[deque[head]]);
        }
    }
    putchar('\n');

    free(readings);
    free(deque);
    return 0;
}
