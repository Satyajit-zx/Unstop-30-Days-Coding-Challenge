#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long team;
    long long rating;
    int pos;
} Entry;

long long *waitTime;

/* Sort by team, then by original position */
int compareTeam(const void *a, const void *b) {
    Entry *x = (Entry *)a;
    Entry *y = (Entry *)b;

    if (x->team < y->team) return -1;
    if (x->team > y->team) return 1;

    return x->pos - y->pos;
}

/* Sort leaderboard:
   larger waiting time first,
   earlier position first if tied
*/
int compareLeaderboard(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    if (waitTime[x] > waitTime[y]) return -1;
    if (waitTime[x] < waitTime[y]) return 1;

    return x - y;
}

int main() {
    int n, K;
    scanf("%d %d", &n, &K);

    Entry *a = (Entry *)malloc(n * sizeof(Entry));
    waitTime = (long long *)malloc(n * sizeof(long long));

    int i;

    for (i = 0; i < n; i++) {
        scanf("%lld %lld", &a[i].team, &a[i].rating);
        a[i].pos = i;
        waitTime[i] = -1;
    }

    /* Group entries by team */
    qsort(a, n, sizeof(Entry), compareTeam);

    int *stack = (int *)malloc(n * sizeof(int));

    int start = 0;

    while (start < n) {
        int end = start + 1;

        while (end < n && a[end].team == a[start].team)
            end++;

        int top = -1;

        /* Process this team's entries from right to left */
        for (i = end - 1; i >= start; i--) {

            /* Remove ratings that cannot be the answer */
            while (top >= 0 &&
                   a[stack[top]].rating <= a[i].rating) {
                top--;
            }

            if (top >= 0) {
                waitTime[a[i].pos] =
                    (long long)a[stack[top]].pos - a[i].pos;
            }

            stack[++top] = i;
        }

        start = end;
    }

    free(stack);
    free(a);

    /* Collect entries having a finite waiting time */
    int *leaderboard = (int *)malloc(n * sizeof(int));
    int count = 0;

    for (i = 0; i < n; i++) {
        if (waitTime[i] != -1) {
            leaderboard[count++] = i;
        }
    }

    /* Sort leaderboard */
    qsort(leaderboard, count, sizeof(int), compareLeaderboard);

    /* First line: waiting figures */
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%lld", waitTime[i]);
    }
    printf("\n");

    /* Second line: top K positions */
    int limit = K < count ? K : count;

    for (i = 0; i < limit; i++) {
        if (i > 0) printf(" ");
        printf("%d", leaderboard[i] + 1);
    }
    printf("\n");

    free(leaderboard);
    free(waitTime);

    return 0;
}
