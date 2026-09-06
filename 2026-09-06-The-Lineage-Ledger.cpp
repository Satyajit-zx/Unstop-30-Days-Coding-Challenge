#include <stdio.h>
#include <stdlib.h>

#define LOG 20

void user_logic(int n, int e[][3], int q, int qu[][2], int res[][2]) {
    int i, j;
    int *p = calloc(n + 1, sizeof(int));
    int *w = calloc(n + 1, sizeof(int));
    int *d = calloc(n + 1, sizeof(int));
    long long *dist = calloc(n + 1, sizeof(long long));

    int *head = malloc((n + 1) * sizeof(int));
    int *to = malloc(n * sizeof(int));
    int *nx = malloc(n * sizeof(int));

    for (i = 0; i <= n; i++) head[i] = -1;

    for (i = 0; i < n - 1; i++) {
        int u = e[i][0], v = e[i][1], x = e[i][2];
        p[v] = u;
        w[v] = x;

        to[i] = v;
        nx[i] = head[u];
        head[u] = i;
    }

    /* Find depth and distance from root 1 */
    int *st = malloc((n + 1) * sizeof(int));
    int top = 0;
    st[top++] = 1;

    while (top) {
        int u = st[--top];
        for (i = head[u]; i != -1; i = nx[i]) {
            int v = to[i];
            d[v] = d[u] + 1;
            dist[v] = dist[u] + w[v];
            st[top++] = v;
        }
    }

    /* Binary lifting */
    int *up = calloc((LOG + 1) * (n + 1), sizeof(int));

    for (i = 1; i <= n; i++)
        up[i] = p[i];

    for (j = 1; j <= LOG; j++)
        for (i = 1; i <= n; i++)
            up[j * (n + 1) + i] =
                up[(j - 1) * (n + 1) +
                   up[(j - 1) * (n + 1) + i]];

    /* Answer queries */
    for (i = 0; i < q; i++) {
        int a = qu[i][0], b = qu[i][1];

        if (d[a] < d[b]) {
            int t = a; a = b; b = t;
        }

        int diff = d[a] - d[b];

        for (j = 0; j <= LOG; j++)
            if (diff & (1 << j))
                a = up[j * (n + 1) + a];

        int lca;

        if (a == b) {
            lca = a;
        } else {
            for (j = LOG; j >= 0; j--) {
                int x = up[j * (n + 1) + a];
                int y = up[j * (n + 1) + b];

                if (x != y) {
                    a = x;
                    b = y;
                }
            }
            lca = p[a];
        }

        res[i][0] =
            (int)(dist[qu[i][0]] +
                  dist[qu[i][1]] -
                  2LL * dist[lca]);

        res[i][1] =
            d[qu[i][0]] +
            d[qu[i][1]] -
            2 * d[lca] + 1;
    }

    free(p);
    free(w);
    free(d);
    free(dist);
    free(head);
    free(to);
    free(nx);
    free(st);
    free(up);
}

int main() {
    int n;
    scanf("%d", &n);

    int edges[n - 1][3];

    for (int i = 0; i < n - 1; i++)
        scanf("%d %d %d",
              &edges[i][0],
              &edges[i][1],
              &edges[i][2]);

    int q;
    scanf("%d", &q);

    int queries[q][2], results[q][2];

    for (int i = 0; i < q; i++)
        scanf("%d %d",
              &queries[i][0],
              &queries[i][1]);

    user_logic(n, edges, q, queries, results);

    for (int i = 0; i < q; i++)
        printf("%d %d\n", results[i][0], results[i][1]);

    return 0;
