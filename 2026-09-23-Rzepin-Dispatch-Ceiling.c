#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF 4000000000000000000LL

typedef long long ll;

typedef struct {
    int to;
    int fuel;
    int turbulence;
    int next;
} Edge;

typedef struct {
    ll dist;
    int node;
    int turb;
} State;

Edge *edges;
int *head;
int edgeCount = 0;

State *heap;
int heapSize = 0;

void swap(State *a, State *b) {
    State temp = *a;
    *a = *b;
    *b = temp;
}

void push(State s) {
    int i = heapSize++;
    heap[i] = s;

    while (i > 0) {
        int p = (i - 1) / 2;

        if (heap[p].dist <= heap[i].dist)
            break;

        swap(&heap[p], &heap[i]);
        i = p;
    }
}

State pop() {
    State result = heap[0];

    heap[0] = heap[--heapSize];

    int i = 0;

    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heapSize &&
            heap[left].dist < heap[smallest].dist)
            smallest = left;

        if (right < heapSize &&
            heap[right].dist < heap[smallest].dist)
            smallest = right;

        if (smallest == i)
            break;

        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }

    return result;
}

void addEdge(int u, int v, int fuel, int turb) {
    edges[edgeCount].to = v;
    edges[edgeCount].fuel = fuel;
    edges[edgeCount].turbulence = turb;
    edges[edgeCount].next = head[u];

    head[u] = edgeCount++;
}

ll solve(int n, int R, int destination, int tolerance) {

    if (tolerance > R)
        tolerance = R;

    /*
       dist[node][turbulence] =
       minimum fuel needed to reach node
       using exactly this turbulence.
    */
    ll *dist = (ll *)malloc(
        (n + 1) * (R + 1) * sizeof(ll)
    );

    int i, j;

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= R; j++) {
            dist[i * (R + 1) + j] = INF;
        }
    }

    int maxStates = (n + 1) * (R + 1);

    heap = (State *)realloc(
        heap,
        maxStates * sizeof(State)
    );

    heapSize = 0;

    dist[1 * (R + 1) + 0] = 0;

    State start;
    start.dist = 0;
    start.node = 1;
    start.turb = 0;

    push(start);

    while (heapSize > 0) {

        State cur = pop();

        ll currentDist =
            dist[cur.node * (R + 1) + cur.turb];

        if (cur.dist != currentDist)
            continue;

        if (cur.node == destination &&
            cur.turb <= tolerance) {

            ll answer = cur.dist;
            free(dist);
            return answer;
        }

        int e = head[cur.node];

        while (e != -1) {

            int newTurb =
                cur.turb + edges[e].turbulence;

            if (newTurb <= tolerance) {

                int v = edges[e].to;

                ll newDist =
                    cur.dist + edges[e].fuel;

                ll *nextDist =
                    &dist[v * (R + 1) + newTurb];

                if (newDist < *nextDist) {

                    *nextDist = newDist;

                    State next;
                    next.dist = newDist;
                    next.node = v;
                    next.turb = newTurb;

                    push(next);
                }
            }

            e = edges[e].next;
        }
    }

    free(dist);
    return -1;
}

int main() {

    int n, m, R;

    scanf("%d %d %d", &n, &m, &R);

    head = (int *)malloc((n + 1) * sizeof(int));

    int i;

    for (i = 0; i <= n; i++)
        head[i] = -1;

    edges = (Edge *)malloc(m * sizeof(Edge));

    for (i = 0; i < m; i++) {

        int u, v, fuel, turb;

        scanf("%d %d %d %d",
              &u, &v, &fuel, &turb);

        addEdge(u, v, fuel, turb);
    }

    int q;
    scanf("%d", &q);

    heap = NULL;

    for (i = 0; i < q; i++) {

        int destination, tolerance;

        scanf("%d %d",
              &destination,
              &tolerance);

        ll answer =
            solve(n, R, destination, tolerance);

        printf("%lld\n", answer);
    }

    free(heap);
    free(edges);
    free(head);

    return 0;
}
