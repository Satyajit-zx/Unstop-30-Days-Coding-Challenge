#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXN 100005
#define LOGN 18

// Adjacency list representation
typedef struct Edge {
    int to;
    struct Edge* next;
} Edge;

Edge* head[MAXN];
Edge edge_pool[MAXN * 2];
int pool_idx = 0;

void add_edge(int u, int v) {
    edge_pool[pool_idx].to = v;
    edge_pool[pool_idx].next = head[u];
    head[u] = &edge_pool[pool_idx++];
}

// Tree metadata
int up[LOGN][MAXN];
int depth[MAXN];
int tin[MAXN];
int tout[MAXN];
int parent_node[MAXN];
int edge_state[MAXN]; // 1 if edge(v, parent[v]) is active, 0 otherwise
int timer = 0;

// Fenwick Tree (Binary Indexed Tree) for range updates and point queries
int bit[MAXN * 2];
int max_bit_idx = 0;

void bit_update(int idx, int val) {
    for (; idx <= max_bit_idx; idx += idx & -idx) {
        bit[idx] += val;
    }
}

int bit_query(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += bit[idx];
    }
    return sum;
}

// Iterative DFS to avoid stack overflow on deep trees
void dfs_iterative(int root, int n) {
    int* stack_node = (int*)malloc((n + 1) * sizeof(int));
    Edge** stack_edge = (Edge**)malloc((n + 1) * sizeof(Edge*));
    int top = 0;

    stack_node[top] = root;
    stack_edge[top] = head[root];
    depth[root] = 0;
    parent_node[root] = 0;
    up[0][root] = root;
    tin[root] = ++timer;

    while (top >= 0) {
        int u = stack_node[top];
        Edge* cur = stack_edge[top];

        if (cur != NULL) {
            stack_edge[top] = cur->next;
            int v = cur->to;
            if (v != up[0][u]) {
                depth[v] = depth[u] + 1;
                parent_node[v] = u;
                up[0][v] = u;
                tin[v] = ++timer;
                edge_state[v] = 1;

                top++;
                stack_node[top] = v;
                stack_edge[top] = head[v];
            }
        } else {
            tout[u] = ++timer;
            top--;
        }
    }

    free(stack_node);
    free(stack_edge);
}

void init_lca(int n) {
    for (int k = 1; k < LOGN; k++) {
        for (int i = 1; i <= n; i++) {
            up[k][i] = up[k - 1][up[k - 1][i]];
        }
    }
}

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) {
        int temp = u; u = v; v = temp;
    }
    for (int k = LOGN - 1; k >= 0; k--) {
        if (depth[u] - (1 << k) >= depth[v]) {
            u = up[k][u];
        }
    }
    if (u == v) return u;

    for (int k = LOGN - 1; k >= 0; k--) {
        if (up[k][u] != up[k][v]) {
            u = up[k][u];
            v = up[k][v];
        }
    }
    return up[0][u];
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }

    dfs_iterative(1, n);
    init_lca(n);

    max_bit_idx = timer;

    // Initially, each edge is electrified (weight = 1)
    for (int v = 2; v <= n; v++) {
        bit_update(tin[v], 1);
        bit_update(tout[v] + 1, -1);
    }

    int q;
    if (scanf("%d", &q) != 1) return 0;

    while (q--) {
        int type;
        scanf("%d", &type);

        if (type == 1) {
            int v;
            scanf("%d", &v);
            if (v == 1) continue; // Depot has no parent edge

            int delta = (edge_state[v] == 1) ? -1 : 1;
            edge_state[v] ^= 1;

            // Subtree range update
            bit_update(tin[v], delta);
            bit_update(tout[v] + 1, -delta);
        } else {
            int u, v;
            scanf("%d %d", &u, &v);
            int lca = get_lca(u, v);
            int dist_u = bit_query(tin[u]);
            int dist_v = bit_query(tin[v]);
            int dist_lca = bit_query(tin[lca]);

            int ans = dist_u + dist_v - 2 * dist_lca;
            printf("%d\n", ans);
        }
    }

    return 0;
}
