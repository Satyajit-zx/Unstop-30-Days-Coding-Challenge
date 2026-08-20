#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005

long long tree[4 * MAXN];
long long lazy[4 * MAXN];
long long a[MAXN];

static inline long long max_val(long long x, long long y) {
    return (x > y) ? x : y;
}

void build(int node, int start, int end) {
    lazy[node] = 0;
    if (start == end) {
        tree[node] = a[start];
        return;
    }
    int mid = start + (end - start) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    tree[node] = max_val(tree[2 * node], tree[2 * node + 1]);
}

void push_down(int node) {
    if (lazy[node] != 0) {
        long long val = lazy[node];
        tree[2 * node] += val;
        lazy[2 * node] += val;
        tree[2 * node + 1] += val;
        lazy[2 * node + 1] += val;
        lazy[node] = 0;
    }
}

void update_range(int node, int start, int end, int l, int r, long long val) {
    if (r < start || end < l) return;
    if (l <= start && end <= r) {
        tree[node] += val;
        lazy[node] += val;
        return;
    }
    push_down(node);
    int mid = start + (end - start) / 2;
    update_range(2 * node, start, mid, l, r, val);
    update_range(2 * node + 1, mid + 1, end, l, r, val);
    tree[node] = max_val(tree[2 * node], tree[2 * node + 1]);
}

long long query_max(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return -1;
    if (l <= start && end <= r) return tree[node];
    
    push_down(node);
    int mid = start + (end - start) / 2;
    long long left_max = query_max(2 * node, start, mid, l, r);
    long long right_max = query_max(2 * node + 1, mid + 1, end, l, r);
    return max_val(left_max, right_max);
}

int query_first_greater(int node, int start, int end, int l, int r, long long x) {
    if (r < start || end < l || tree[node] <= x) {
        return -1;
    }
    if (start == end) {
        return start;
    }
    
    push_down(node);
    int mid = start + (end - start) / 2;
    
    int res = query_first_greater(2 * node, start, mid, l, r, x);
    if (res != -1) return res;
    return query_first_greater(2 * node + 1, mid + 1, end, l, r, x);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
    }
    
    build(1, 1, n);
    
    int q;
    if (scanf("%d", &q) != 1) return 0;
    
    while (q--) {
        int type;
        scanf("%d", &type);
        if (type == 1) {
            int l, r;
            long long v;
            scanf("%d %d %lld", &l, &r, &v);
            update_range(1, 1, n, l, r, v);
        } else if (type == 2) {
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%lld\n", query_max(1, 1, n, l, r));
        } else if (type == 3) {
            int l, r;
            long long x;
            scanf("%d %d %lld", &l, &r, &x);
            printf("%d\n", query_first_greater(1, 1, n, l, r, x));
        }
    }
    
    return 0;
}
