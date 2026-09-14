#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXN 20005

typedef struct Edge {
    int to;
    long long weight;
    struct Edge* next;
} Edge;

Edge* adj[MAXN];
bool removed[MAXN];
int subtree_size[MAXN];

void add_edge(int u, int v, long long w) {
    Edge* e1 = (Edge*)malloc(sizeof(Edge));
    e1->to = v;
    e1->weight = w;
    e1->next = adj[u];
    adj[u] = e1;

    Edge* e2 = (Edge*)malloc(sizeof(Edge));
    e2->to = u;
    e2->weight = w;
    e2->next = adj[v];
    adj[v] = e2;
}

// Compute subtree sizes
void get_sizes(int u, int p) {
    subtree_size[u] = 1;
    for (Edge* e = adj[u]; e != NULL; e = e->next) {
        int v = e->to;
        if (v != p && !removed[v]) {
            get_sizes(v, u);
            subtree_size[u] += subtree_size[v];
        }
    }
}

// Find centroid of the component
int get_centroid(int u, int p, int total_nodes) {
    for (Edge* e = adj[u]; e != NULL; e = e->next) {
        int v = e->to;
        if (v != p && !removed[v] && subtree_size[v] > total_nodes / 2) {
            return get_centroid(v, u, total_nodes);
        }
    }
    return u;
}

// Collect path distances
void get_distances(int u, int p, long long d, long long* dist_arr, int* count) {
    dist_arr[(*count)++] = d;
    for (Edge* e = adj[u]; e != NULL; e = e->next) {
        int v = e->to;
        if (v != p && !removed[v]) {
            get_distances(v, u, d + e->weight, dist_arr, count);
        }
    }
}

int cmp_ll(const void* a, const void* b) {
    long long diff = (*(long long*)a - *(long long*)b);
    return (diff > 0) - (diff < 0);
}

// Count pairs in dist_arr where dist_arr[i] + dist_arr[j] <= D (i < j)
long long count_pairs(long long* arr, int len, long long D) {
    qsort(arr, len, sizeof(long long), cmp_ll);
    long long count = 0;
    int left = 0, right = len - 1;
    while (left < right) {
        if (arr[left] + arr[right] <= D) {
            count += (right - left);
            left++;
        } else {
            right--;
        }
    }
    return count;
}

static long long all_dists[MAXN];
static long long sub_dists[MAXN];

long long solve_centroid(int u, long long D) {
    get_sizes(u, 0);
    int c = get_centroid(u, 0, subtree_size[u]);
    removed[c] = true;

    long long ans = 0;
    int all_count = 0;

    // Collect distances across all subtrees connected to c
    for (Edge* e = adj[c]; e != NULL; e = e->next) {
        int v = e->to;
        if (!removed[v]) {
            int sub_count = 0;
            get_distances(v, c, e->weight, sub_dists, &sub_count);

            // Subtract pairs in the same branch
            ans -= count_pairs(sub_dists, sub_count, D);

            // Add branch distances to all_dists
            for (int i = 0; i < sub_count; ++i) {
                all_dists[all_count++] = sub_dists[i];
                if (sub_dists[i] <= D) {
                    ans++; // Pair formed with centroid `c` itself
                }
            }
        }
    }

    // Add valid pairs across all branches combined
    ans += count_pairs(all_dists, all_count, D);

    // Recurse on remaining components
    for (Edge* e = adj[c]; e != NULL; e = e->next) {
        int v = e->to;
        if (!removed[v]) {
            ans += solve_centroid(v, D);
        }
    }

    return ans;
}

long long countSensorPairs(int n, long long D, int edges[][3]) {
    for (int i = 1; i <= n; ++i) {
        adj[i] = NULL;
        removed[i] = false;
    }

    for (int i = 0; i < n - 1; ++i) {
        add_edge(edges[i][0], edges[i][1], (long long)edges[i][2]);
    }

    return solve_centroid(1, D);
}

int main() {
    int n;
    long long D;
    if (scanf("%d %lld", &n, &D) != 2) return 0;

    int (*edges)[3] = malloc((n - 1) * sizeof(*edges));
    for (int i = 0; i < n - 1; ++i) {
        scanf("%d %d %d", &edges[i][0], &edges[i][1], &edges[i][2]);
    }

    long long result = countSensorPairs(n, D, edges);
    printf("%lld\n", result);

    free(edges);
    return 0;
}
