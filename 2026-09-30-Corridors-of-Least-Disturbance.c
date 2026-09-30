#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005
#define LOG 20

typedef struct {
    int u, v, w;
} Edge;

typedef struct {
    int to, w, next;
} Node;

Edge edges[MAXN];
Node graph[2 * MAXN];

int parent[MAXN];
int head[MAXN];
int depth[MAXN];

int up[LOG][MAXN];
int mx[LOG][MAXN];

int edgeCount = 0;

/* DSU Find */
int find(int x) {
    if (parent[x] == x)
        return x;

    parent[x] = find(parent[x]);
    return parent[x];
}

/* DSU Union */
void unite(int a, int b) {
    a = find(a);
    b = find(b);

    if (a != b)
        parent[b] = a;
}

/* Sort edges by weight */
int compareEdges(const void *a, const void *b) {
    Edge *x = (Edge *)a;
    Edge *y = (Edge *)b;

    return x->w - y->w;
}

/* Add edge to MST */
void addEdge(int u, int v, int w) {
    graph[edgeCount].to = v;
    graph[edgeCount].w = w;
    graph[edgeCount].next = head[u];

    head[u] = edgeCount++;
}

/* DFS */
void dfs(int u, int p, int weight) {
    up[0][u] = p;
    mx[0][u] = weight;

    for (int e = head[u]; e != -1; e = graph[e].next) {
        int v = graph[e].to;

        if (v == p)
            continue;

        depth[v] = depth[u] + 1;

        dfs(v, u, graph[e].w);
    }
}

/* Maximum edge on path */
int query(int u, int v) {

    if (find(u) != find(v))
        return -1;

    int ans = 0;

    /* Bring u and v to same level */
    if (depth[u] < depth[v]) {
        int temp = u;
        u = v;
        v = temp;
    }

    int diff = depth[u] - depth[v];

    for (int j = LOG - 1; j >= 0; j--) {

        if (diff & (1 << j)) {

            if (mx[j][u] > ans)
                ans = mx[j][u];

            u = up[j][u];
        }
    }

    if (u == v)
        return ans;

    /* Lift both nodes */
    for (int j = LOG - 1; j >= 0; j--) {

        if (up[j][u] != up[j][v]) {

            if (mx[j][u] > ans)
                ans = mx[j][u];

            if (mx[j][v] > ans)
                ans = mx[j][v];

            u = up[j][u];
            v = up[j][v];
        }
    }

    if (mx[0][u] > ans)
        ans = mx[0][u];

    if (mx[0][v] > ans)
        ans = mx[0][v];

    return ans;
}

int main() {

    int n, m;

    /* First line: N M */
    scanf("%d %d", &n, &m);

    /* Initialize */
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        head[i] = -1;
    }

    /* Read edges */
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d",
              &edges[i].u,
              &edges[i].v,
              &edges[i].w);
    }

    /* Sort edges */
    qsort(edges, m, sizeof(Edge), compareEdges);

    /* Build Minimum Spanning Forest */
    for (int i = 0; i < m; i++) {

        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;

        if (find(u) != find(v)) {

            unite(u, v);

            addEdge(u, v, w);
            addEdge(v, u, w);
        }
    }

    /* Prepare LCA */
    for (int i = 1; i <= n; i++) {

        if (head[i] != -1 && up[0][i] == 0) {

            depth[i] = 0;
            dfs(i, i, 0);
        }
    }

    /* Binary lifting */
    for (int j = 1; j < LOG; j++) {

        for (int i = 1; i <= n; i++) {

            int mid = up[j - 1][i];

            up[j][i] = up[j - 1][mid];

            mx[j][i] = mx[j - 1][i];

            if (mx[j - 1][mid] > mx[j][i])
                mx[j][i] = mx[j - 1][mid];
        }
    }

    /*
       IMPORTANT:
       Q comes AFTER all edges
    */
    int q;
    scanf("%d", &q);

    while (q--) {

        int u, v;
        scanf("%d %d", &u, &v);

        printf("%d\n", query(u, v));
    }

    return 0;
}
