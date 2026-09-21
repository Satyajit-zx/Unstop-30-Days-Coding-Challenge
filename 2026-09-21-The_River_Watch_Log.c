#include <stdio.h>
#include <stdlib.h>

int cmpLL(const void *a, const void *b) {
    long long x = *(const long long *)a;
    long long y = *(const long long *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int lower_bound(long long arr[], int n, long long x) {
    int l = 0, r = n;

    while (l < r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] < x)
            l = mid + 1;
        else
            r = mid;
    }

    return l;
}

int main() {
    int n, W;
    scanf("%d %d", &n, &W);

    long long *a = (long long *)malloc(n * sizeof(long long));
    long long *sorted = (long long *)malloc(n * sizeof(long long));

    int i;

    for (i = 0; i < n; i++) {
        scanf("%lld", &a[i]);
        sorted[i] = a[i];
    }

    /* Coordinate compression */
    qsort(sorted, n, sizeof(long long), cmpLL);

    int unique = 0;

    for (i = 0; i < n; i++) {
        if (i == 0 || sorted[i] != sorted[i - 1]) {
            sorted[unique++] = sorted[i];
        }
    }

    int *freq = (int *)calloc(unique, sizeof(int));

    /* Deque for maximum */
    int *dq = (int *)malloc(n * sizeof(int));
    int front = 0, back = 0;

    int left = 0;
    int distinct = 0;

    for (i = 0; i < n; i++) {

        /* Add current element */
        int id = lower_bound(sorted, unique, a[i]);

        if (freq[id] == 0)
            distinct++;

        freq[id]++;

        /* Maintain decreasing deque */
        while (front < back && a[dq[back - 1]] <= a[i])
            back--;

        dq[back++] = i;

        /* Keep exactly W elements in window */
        if (i - left + 1 > W) {

            int oldId = lower_bound(sorted, unique, a[left]);

            freq[oldId]--;

            if (freq[oldId] == 0)
                distinct--;

            left++;
        }

        /* Remove indices outside window from deque */
        while (front < back && dq[front] < left)
            front++;

        /* Window is ready */
        if (i - left + 1 == W) {
            long long maximum = a[dq[front]];

            printf("%lld %d\n", maximum, distinct);
        }
    }

    free(a);
    free(sorted);
    free(freq);
    free(dq);

    return 0;
}
