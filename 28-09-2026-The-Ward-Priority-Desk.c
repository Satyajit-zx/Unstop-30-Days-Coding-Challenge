#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXQ 200005

typedef struct {
    char type[10];
    long long id;
    long long priority;
} Command;

typedef struct {
    long long id;
    long long priority;
    int arrival;
    int version;
} Node;

Command cmd[MAXQ];
long long ids[MAXQ];
Node heap[MAXQ];

int heapSize = 0;

int active[MAXQ];
int versionArr[MAXQ];
int arrivalArr[MAXQ];

int better(Node a, Node b) {
    if (a.priority != b.priority)
        return a.priority > b.priority;

    /* Same priority -> earlier request first */
    return a.arrival < b.arrival;
}

void swapNode(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void push(Node x) {
    int i = heapSize++;
    heap[i] = x;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (better(heap[parent], heap[i]))
            break;

        swapNode(&heap[parent], &heap[i]);
        i = parent;
    }
}

Node popNode() {
    Node result = heap[0];
    heapSize--;

    if (heapSize > 0) {
        heap[0] = heap[heapSize];

        int i = 0;

        while (1) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int best = i;

            if (left < heapSize &&
                better(heap[left], heap[best]))
                best = left;

            if (right < heapSize &&
                better(heap[right], heap[best]))
                best = right;

            if (best == i)
                break;

            swapNode(&heap[i], &heap[best]);
            i = best;
        }
    }

    return result;
}

int compareLL(const void *a, const void *b) {
    long long x = *(const long long *)a;
    long long y = *(const long long *)b;

    if (x < y)
        return -1;
    if (x > y)
        return 1;

    return 0;
}

int findId(long long id, int n) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (ids[mid] == id)
            return mid;

        if (ids[mid] < id)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

int main() {
    int q;

    scanf("%d", &q);

    int idCount = 0;

    /* Read all commands */
    for (int i = 0; i < q; i++) {

        scanf("%9s", cmd[i].type);

        if (strcmp(cmd[i].type, "DISPATCH") == 0) {
            cmd[i].id = 0;
            cmd[i].priority = 0;
        }
        else if (strcmp(cmd[i].type, "ADD") == 0 ||
                 strcmp(cmd[i].type, "UPDATE") == 0) {

            scanf("%lld %lld",
                  &cmd[i].id,
                  &cmd[i].priority);

            ids[idCount++] = cmd[i].id;
        }
        else if (strcmp(cmd[i].type, "CANCEL") == 0) {

            scanf("%lld", &cmd[i].id);

            cmd[i].priority = 0;
            ids[idCount++] = cmd[i].id;
        }
    }

    /* Coordinate compression of IDs */
    qsort(ids, idCount, sizeof(long long), compareLL);

    int n = 0;

    for (int i = 0; i < idCount; i++) {
        if (i == 0 || ids[i] != ids[i - 1])
            ids[n++] = ids[i];
    }

    int arrivalCounter = 0;

    /* Process commands */
    for (int i = 0; i < q; i++) {

        int idx;

        /* ADD id priority */
        if (strcmp(cmd[i].type, "ADD") == 0) {

            idx = findId(cmd[i].id, n);

            active[idx] = 1;

            arrivalArr[idx] = ++arrivalCounter;

            versionArr[idx]++;

            Node x;

            x.id = cmd[i].id;
            x.priority = cmd[i].priority;
            x.arrival = arrivalArr[idx];
            x.version = versionArr[idx];

            push(x);
        }

        /* UPDATE id priority */
        else if (strcmp(cmd[i].type, "UPDATE") == 0) {

            idx = findId(cmd[i].id, n);

            if (idx >= 0 && active[idx]) {

                versionArr[idx]++;

                Node x;

                x.id = cmd[i].id;
                x.priority = cmd[i].priority;

                /* Update does not change arrival order */
                x.arrival = arrivalArr[idx];

                x.version = versionArr[idx];

                push(x);
            }
        }

        /* CANCEL id */
        else if (strcmp(cmd[i].type, "CANCEL") == 0) {

            idx = findId(cmd[i].id, n);

            if (idx >= 0 && active[idx]) {

                active[idx] = 0;
                versionArr[idx]++;
            }
        }

        /* DISPATCH */
        else if (strcmp(cmd[i].type, "DISPATCH") == 0) {

            int served = 0;

            while (heapSize > 0) {

                Node x = popNode();

                idx = findId(x.id, n);

                /* Cancelled request */
                if (!active[idx])
                    continue;

                /* Old heap entry after UPDATE */
                if (versionArr[idx] != x.version)
                    continue;

                printf("%lld\n", x.id);

                active[idx] = 0;
                versionArr[idx]++;

                served = 1;

                break;
            }

            /* No pending request */
            if (!served)
                printf("-1\n");
        }
    }

    return 0;
}
