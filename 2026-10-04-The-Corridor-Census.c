#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int l;
    int r;
    int id;
} Query;

int *arr;
int *freq;
int *freqCount;
int currentMax = 0;
int blockSize;

int compareInt(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int lowerBound(int *a, int n, int value) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (a[mid] == value)
            return mid;
        else if (a[mid] < value)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return low;
}

int compareQueries(const void *a, const void *b) {
    Query *x = (Query *)a;
    Query *y = (Query *)b;

    int bx = x->l / blockSize;
    int by = y->l / blockSize;

    if (bx != by)
        return bx - by;

    if (bx & 1)
        return y->r - x->r;

    return x->r - y->r;
}

void addElement(int value) {
    int oldFreq = freq[value];

    if (oldFreq > 0)
        freqCount[oldFreq]--;

    freq[value]++;

    freqCount[freq[value]]++;

    if (freq[value] > currentMax)
        currentMax = freq[value];
}

void removeElement(int value) {
    int oldFreq = freq[value];

    freqCount[oldFreq]--;

    freq[value]--;

    if (freq[value] > 0)
        freqCount[freq[value]]++;

    while (currentMax > 0 && freqCount[currentMax] == 0)
        currentMax--;
}

int main() {
    int n, q;
    int i;

    scanf("%d %d", &n, &q);

    arr = (int *)malloc(n * sizeof(int));

    /* Copy for coordinate compression */
    int *values = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        values[i] = arr[i];
    }

    /* Sort values */
    qsort(values, n, sizeof(int), compareInt);

    /* Remove duplicates */
    int uniqueCount = 0;

    for (i = 0; i < n; i++) {
        if (i == 0 || values[i] != values[i - 1]) {
            values[uniqueCount++] = values[i];
        }
    }

    /* Convert original values into compressed indexes */
    for (i = 0; i < n; i++) {
        arr[i] = lowerBound(values, uniqueCount, arr[i]);
    }

    free(values);

    Query *queries = (Query *)malloc(q * sizeof(Query));
    int *answer = (int *)malloc(q * sizeof(int));

    for (i = 0; i < q; i++) {
        int l, r;

        scanf("%d %d", &l, &r);

        queries[i].l = l - 1;
        queries[i].r = r - 1;
        queries[i].id = i;
    }

    /* Frequency of each compressed value */
    freq = (int *)calloc(uniqueCount, sizeof(int));

    /* freqCount[x] = how many values occur exactly x times */
    freqCount = (int *)calloc(n + 1, sizeof(int));

    blockSize = (int)sqrt((double)n);

    qsort(queries, q, sizeof(Query), compareQueries);

    int left = 0;
    int right = -1;

    for (i = 0; i < q; i++) {
        int L = queries[i].l;
        int R = queries[i].r;

        while (right < R) {
            right++;
            addElement(arr[right]);
        }

        while (right > R) {
            removeElement(arr[right]);
            right--;
        }

        while (left < L) {
            removeElement(arr[left]);
            left++;
        }

        while (left > L) {
            left--;
            addElement(arr[left]);
        }

        answer[queries[i].id] = currentMax;
    }

    /* Print answers in original query order */
    for (i = 0; i < q; i++) {
        printf("%d\n", answer[i]);
    }

    free(arr);
    free(freq);
    free(freqCount);
    free(queries);
    free(answer);

    return 0;
}
