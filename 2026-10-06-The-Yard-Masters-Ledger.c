#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

typedef struct {
    int deadline;
    ll importance;
    int index;
} Request;

int *parent;

int find(int x) {
    if (parent[x] == x)
        return x;

    parent[x] = find(parent[x]);
    return parent[x];
}

int compare(const void *a, const void *b) {
    const Request *x = (const Request *)a;
    const Request *y = (const Request *)b;

    if (x->importance > y->importance)
        return -1;

    if (x->importance < y->importance)
        return 1;

    return x->index - y->index;
}

int main() {
    int D, T;

    scanf("%d %d", &D, &T);

    int *capacity = (int *)malloc((D + 1) * sizeof(int));

    for (int i = 1; i <= D; i++) {
        scanf("%d", &capacity[i]);
    }

    Request *req = (Request *)malloc(T * sizeof(Request));

    for (int i = 0; i < T; i++) {
        scanf("%d %lld", &req[i].deadline, &req[i].importance);
        req[i].index = i;
    }

    /* Sort by importance - highest first */
    qsort(req, T, sizeof(Request), compare);

    parent = (int *)malloc((D + 1) * sizeof(int));

    for (int i = 0; i <= D; i++) {
        parent[i] = i;
    }

    /*
       Remove days having zero capacity initially.
    */
    for (int i = 1; i <= D; i++) {
        if (capacity[i] == 0) {
            parent[i] = find(i - 1);
        }
    }

    int *answer = (int *)calloc(T, sizeof(int));

    ll totalImportance = 0;

    for (int i = 0; i < T; i++) {

        int deadline = req[i].deadline;

        if (deadline > D)
            deadline = D;

        if (deadline < 1)
            continue;

        /* Find latest available day <= deadline */
        int day = find(deadline);

        if (day >= 1 && capacity[day] > 0) {

            answer[req[i].index] = day;

            totalImportance += req[i].importance;

            capacity[day]--;

            /*
               If this day is full, remove it from DSU.
            */
            if (capacity[day] == 0) {
                parent[day] = find(day - 1);
            }
        }
    }

    printf("%lld\n", totalImportance);

    for (int i = 0; i < T; i++) {
        printf("%d", answer[i]);

        if (i < T - 1)
            printf(" ");
    }

    printf("\n");

    free(capacity);
    free(req);
    free(answer);
    free(parent);

    return 0;
}
