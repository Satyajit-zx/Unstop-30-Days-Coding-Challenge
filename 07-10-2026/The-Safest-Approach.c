#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;

typedef struct Edge {
    int to;
    int w;
    int next;
} Edge;

typedef struct {
    int node;
    ll dist;
} HeapNode;

typedef struct {
    HeapNode *a;
    int size;
    int capacity;
} MinHeap;

Edge *edges;
int *head;
int edgeCount = 0;

/* ---------- Graph ---------- */

void addEdge(int u, int v, int w) {
    edges[edgeCount].to = v;
    edges[edgeCount].w = w;
    edges[edgeCount].next = head[u];
    head[u] = edgeCount++;
}

/* ---------- Min Heap ---------- */

void swapHeap(HeapNode *x, HeapNode *y) {
    HeapNode temp = *x;
    *x = *y;
    *y = temp;
}

void heapPush(MinHeap *h, int node, ll dist) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->a = (HeapNode *)realloc(
            h->a,
            h->capacity * sizeof(HeapNode)
        );
    }

    int i = h->size++;

    h->a[i].node = node;
    h->a[i].dist = dist;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (h->a[parent].dist <= h->a[i].dist)
            break;

        swapHeap(&h->a[parent], &h->a[i]);
        i = parent;
    }
}

HeapNode heapPop(MinHeap *h) {
    HeapNode result = h->a[0];

    h->size--;

    if (h->size > 0) {
        h->a[0] = h->a[h->size];

        int i = 0;

        while (1) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < h->size &&
                h->a[left].dist < h->a[smallest].dist) {
                smallest = left;
            }

            if (right < h->size &&
                h->a[right].dist < h->a[smallest].dist) {
                smallest = right;
            }

            if (smallest == i)
                break;

            swapHeap(&h->a[i], &h->a[smallest]);
            i = smallest;
        }
    }

    return result;
}

int heapEmpty(MinHeap *h) {
    return h->size == 0;
}

/* ---------- Main ---------- */

int main() {
    int n, m;

    scanf("%d %d", &n, &m);

    head = (int *)malloc((n + 1) * sizeof(int));

    for (int i = 1; i <= n; i++)
        head[i] = -1;

    /*
       Undirected graph:
       each tube can be used in either direction.
    */
    edges = (Edge *)malloc(
        (2LL * m) * sizeof(Edge)
    );

    for (int i = 0; i < m; i++) {
        int u, v, w;

        scanf("%d %d %d", &u, &v, &w);

        addEdge(u, v, w);
        addEdge(v, u, w);
    }

    /*
       dist[v] =
       minimum possible maximum edge weight
       on any path from module 1 to v.
    */
    ll *dist = (ll *)malloc(
        (n + 1) * sizeof(ll)
    );

    for (int i = 1; i <= n; i++)
        dist[i] = LLONG_MAX;

    dist[1] = 0;

    MinHeap heap;

    heap.size = 0;
    heap.capacity = 1024;
    heap.a = (HeapNode *)malloc(
        heap.capacity * sizeof(HeapNode)
    );

    heapPush(&heap, 1, 0);

    while (!heapEmpty(&heap)) {
        HeapNode cur = heapPop(&heap);

        int u = cur.node;
        ll d = cur.dist;

        /*
           Ignore outdated heap entries.
        */
        if (d != dist[u])
            continue;

        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            ll w = edges[e].w;

            /*
               If we extend the path with edge w,
               the new danger is the larger of:
               current danger and w.
            */
            ll newDist = (d > w) ? d : w;

            if (newDist < dist[v]) {
                dist[v] = newDist;

                heapPush(
                    &heap,
                    v,
                    newDist
                );
            }
        }
    }

    /* Output answers */
    for (int i = 1; i <= n; i++) {
        if (i > 1)
            printf(" ");

        if (dist[i] == LLONG_MAX)
            printf("-1");
        else
            printf("%lld", dist[i]);
    }

    printf("\n");

    free(head);
    free(edges);
    free(dist);
    free(heap.a);

    return 0;
}
