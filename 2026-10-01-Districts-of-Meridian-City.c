#include <stdio.h>
#include <string.h>

#define MAXN 200005
#define MAXL 20

int parent[MAXN];
int rating[MAXN];
int topRating[MAXN];

char code[MAXN][MAXL];

int find(int x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]);

    return parent[x];
}

void linkDistricts(int a, int b) {
    a = find(a);
    b = find(b);

    if (a == b)
        return;

    parent[b] = a;

    if (topRating[b] > topRating[a])
        topRating[a] = topRating[b];
}

int getIndex(char *name, int n) {
    int i;

    for (i = 0; i < n; i++) {
        if (strcmp(code[i], name) == 0)
            return i;
    }

    return -1;
}

int main() {
    int n, q;
    int i;

    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        scanf("%s %d", code[i], &rating[i]);

        parent[i] = i;
        topRating[i] = rating[i];
    }

    scanf("%d", &q);

    for (i = 0; i < q; i++) {
        char command[10];
        char x[MAXL], y[MAXL];
        int value;
        int a, b, root;

        scanf("%s", command);

        if (strcmp(command, "QUERY") == 0) {

            scanf("%s", x);

            a = getIndex(x, n);
            root = find(a);

            printf("%d\n", topRating[root]);
        }

        else if (strcmp(command, "LINK") == 0) {

            scanf("%s %s", x, y);

            a = getIndex(x, n);
            b = getIndex(y, n);

            linkDistricts(a, b);
        }

        else if (strcmp(command, "BOOST") == 0) {

            scanf("%s %d", x, &value);

            a = getIndex(x, n);

            /* Increase individual district rating */
            rating[a] += value;

            /* Update maximum rating of its zone */
            root = find(a);

            if (rating[a] > topRating[root])
                topRating[root] = rating[a];
        }
    }

    return 0;
}
