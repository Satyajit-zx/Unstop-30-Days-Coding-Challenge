#include <stdio.h>
#include <stdlib.h>

// Comparison function for sorting integers
int cmp(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

// Binary search to find compressed index
int lower_bound(int *arr, int size, int target) {
    int left = 0, right = size - 1;
    int ans = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] >= target) {
            ans = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return ans;
}

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    int *arr = (int *)malloc(n * sizeof(int));
    int *temp = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        temp[i] = arr[i];
    }

    // Step 1: Coordinate Compression
    qsort(temp, n, sizeof(int), cmp);

    int unique_count = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || temp[i] != temp[i - 1]) {
            temp[unique_count++] = temp[i];
        }
    }

    for (int i = 0; i < n; i++) {
        arr[i] = lower_bound(temp, unique_count, arr[i]);
    }

    // Step 2: Sliding Window (Two Pointers)
    int *freq = (int *)calloc(unique_count, sizeof(int));
    int distinct = 0;
    int left = 0;
    int max_len = 0;

    for (int right = 0; right < n; right++) {
        if (freq[arr[right]] == 0) {
            distinct++;
        }
        freq[arr[right]]++;

        while (distinct > k) {
            freq[arr[left]]--;
            if (freq[arr[left]] == 0) {
                distinct--;
            }
            left++;
        }

        int current_len = right - left + 1;
        if (current_len > max_len) {
            max_len = current_len;
        }
    }

    printf("%d\n", max_len);

    // Free allocated memory
    free(arr);
    free(temp);
    free(freq);

    return 0;
}
