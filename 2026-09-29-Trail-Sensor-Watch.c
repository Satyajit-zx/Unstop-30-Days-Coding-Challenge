#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int lowerBound(int arr[], int n, int x) {
    int left = 0, right = n;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] < x)
            left = mid + 1;
        else
            right = mid;
    }

    return left;
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);

    int *v = (int *)malloc(n * sizeof(int));
    int *s = (int *)malloc(n * sizeof(int));
    int *sorted = (int *)malloc(n * sizeof(int));
    int *id = (int *)malloc(n * sizeof(int));
    int *freq = (int *)calloc(n, sizeof(int));
    int *dq = (int *)malloc(n * sizeof(int));

    int i;

    for (i = 0; i < n; i++)
        scanf("%d", &v[i]);

    for (i = 0; i < n; i++) {
        scanf("%d", &s[i]);
        sorted[i] = s[i];
    }

    /* Coordinate compression */
    qsort(sorted, n, sizeof(int), cmp);

    int unique = 0;

    for (i = 0; i < n; i++) {
        if (i == 0 || sorted[i] != sorted[i - 1])
            sorted[unique++] = sorted[i];
    }

    for (i = 0; i < n; i++)
        id[i] = lowerBound(sorted, unique, s[i]);

    /*
       A stretch is notable if distinct species
       is at least ceil(k / 2).
    */
    int required = (k + 1) / 2;

    int distinct = 0;
    int front = 0, back = 0;

    for (i = 0; i < n; i++) {

        /* Add current species */
        if (freq[id[i]] == 0)
            distinct++;

        freq[id[i]]++;

        /*
           Maintain DECREASING deque
           so front contains maximum reading.
        */
        while (front < back &&
               v[dq[back - 1]] <= v[i]) {
            back--;
        }

        dq[back++] = i;

        /* Remove element outside window */
        if (i >= k) {
            int old = i - k;

            freq[id[old]]--;

            if (freq[id[old]] == 0)
                distinct--;

            if (front < back && dq[front] == old)
                front++;
        }

        /* Complete window */
        if (i >= k - 1) {

            if (distinct >= required)
                printf("%d", v[dq[front]]);
            else
                printf("-1");

            if (i != n - 1)
                printf(" ");
        }
    }

    printf("\n");

    free(v);
    free(s);
    free(sorted);
    free(id);
    free(freq);
    free(dq);

    return 0;
}
