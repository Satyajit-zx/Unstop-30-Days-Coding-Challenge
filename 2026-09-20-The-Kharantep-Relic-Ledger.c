#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 200005

typedef struct Node {
    long long value;
    int id;
    int left;
    int right;
} Node;

Node heap[MAXN];
int parent[MAXN];
int sz[MAXN];

/* Higher value first.
   If values are equal, smaller chamber number first. */
int better(int a, int b) {
    if (heap[a].value != heap[b].value)
        return heap[a].value > heap[b].value;

    return heap[a].id < heap[b].id;
}

/* Meldable skew heap */
int meld(int a, int b) {
    int temp;

    if (a == 0)
        return b;
    if (b == 0)
        return a;

    if (!better(a, b)) {
        temp = a;
        a = b;
        b = temp;
    }

    heap[a].right = meld(heap[a].right, b);

    temp = heap[a].left;
    heap[a].left = heap[a].right;
    heap[a].right = temp;

    return a;
}

int findRoot(int x) {
    if (parent[x] != x)
        parent[x] = findRoot(parent[x]);

    return parent[x];
}

void popHeap(int *root) {
    int old = *root;

    *root = meld(heap[old].left, heap[old].right);

    heap[old].left = 0;
    heap[old].right = 0;
}

int main() {
    int n, m;
    int i;

    scanf("%d %d", &n, &m);

    for (i = 1; i <= n; i++) {
        long long value;

        scanf("%lld", &value);

        parent[i] = i;
        sz[i] = 1;

        heap[i].value = value;
        heap[i].id = i;
        heap[i].left = 0;
        heap[i].right = 0;

        /* 0 means no relic */
        if (value == 0)
            heap[i] = (Node){0, i, 0, 0};
    }

    /*
       rootHeap[r] stores the relic heap of component r.
       If there is no relic, it is 0.
    */
    static int rootHeap[MAXN];

    for (i = 1; i <= n; i++) {
        if (heap[i].value > 0)
            rootHeap[i] = i;
        else
            rootHeap[i] = 0;
    }

    for (i = 0; i < m; i++) {
        char operation[10];
        int u, v;

        scanf("%s", operation);

        if (strcmp(operation, "LINK") == 0) {
            scanf("%d %d", &u, &v);

            int ru = findRoot(u);
            int rv = findRoot(v);

            if (ru != rv) {
                /* Union by size */
                if (sz[ru] < sz[rv]) {
                    int temp = ru;
                    ru = rv;
                    rv = temp;
                }

                parent[rv] = ru;
                sz[ru] += sz[rv];

                /* Merge both relic heaps */
                rootHeap[ru] = meld(rootHeap[ru], rootHeap[rv]);
                rootHeap[rv] = 0;
            }
        }
        else if (strcmp(operation, "CLAIM") == 0) {
            scanf("%d", &u);

            int r = findRoot(u);
            int h = rootHeap[r];

            if (h == 0) {
                printf("EMPTY\n");
            }
            else {
                /*
                   Output:
                   chamber_number relic_value
                */
                printf("%d %lld\n", heap[h].id, heap[h].value);

                popHeap(&rootHeap[r]);
            }
        }
    }

    return 0;
}
