#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

int cmp(const void *a, const void *b) {
    ll x = *(const ll *)a;
    ll y = *(const ll *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int lower_bound(ll arr[], int n, ll x) {
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

int upper_bound(ll arr[], int n, ll x) {
    int l = 0, r = n;

    while (l < r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] <= x)
            l = mid + 1;
        else
            r = mid;
    }

    return l;
}

void update(ll bit[], int n, int idx, ll val) {
    while (idx <= n) {
        bit[idx] += val;
        idx += idx & (-idx);
    }
}

ll query(ll bit[], int idx) {
    ll sum = 0;

    while (idx > 0) {
        sum += bit[idx];
        idx -= idx & (-idx);
    }

    return sum;
}

int main() {
    int n;
    ll k;

    scanf("%d %lld", &n, &k);

    ll *a = (ll *)malloc(n * sizeof(ll));
    ll *sorted = (ll *)malloc(n * sizeof(ll));
    ll *bit = (ll *)calloc(n + 1, sizeof(ll));

    for (int i = 0; i < n; i++) {
        scanf("%lld", &a[i]);
        sorted[i] = a[i];
    }

    /* Coordinate compression */
    qsort(sorted, n, sizeof(ll), cmp);

    int m = 0;

    for (int i = 0; i < n; i++) {
        if (i == 0 || sorted[i] != sorted[i - 1]) {
            sorted[m++] = sorted[i];
        }
    }

    /*
       Process days from left to right.
       Only previous days are inserted into Fenwick Tree.
    */
    for (int i = 0; i < n; i++) {

        ll low = a[i] - k;
        ll high = a[i] + k;

        /*
           We need previous scores in:
           [a[i] - k, a[i] + k]
        */

        int left = lower_bound(sorted, m, low);
        int right = upper_bound(sorted, m, high);

        /*
           Fenwick indices are 1-based.
           Number of inserted values in [left, right)
        */
        ll count = query(bit, right) - query(bit, left);

        printf("%lld\n", count);

        /* Insert current score for future days */
        int pos = lower_bound(sorted, m, a[i]);

        update(bit, m, pos + 1, 1);
    }

    free(a);
    free(sorted);
    free(bit);

    return 0;
}
