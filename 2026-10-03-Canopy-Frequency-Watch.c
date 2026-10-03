#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXN 200000
#define MAXLEN 15

typedef struct {
    char name[MAXLEN + 1];
    int count;
} Species;

Species a[MAXN];
int m = 0;

int findSpecies(char name[]) {
    int i;

    for (i = 0; i < m; i++) {
        if (strcmp(a[i].name, name) == 0)
            return i;
    }

    return -1;
}

int cmp(const void *x, const void *y) {
    Species *p = (Species *)x;
    Species *q = (Species *)y;

    /* Higher frequency first */
    if (p->count != q->count)
        return q->count - p->count;

    /* Same frequency -> alphabetical */
    return strcmp(p->name, q->name);
}

int main() {
    int n, k;
    int i;

    scanf("%d %d", &n, &k);

    for (i = 0; i < n; i++) {
        char type;
        char name[MAXLEN + 1];

        scanf(" %c", &type);

        if (type == 'S') {
            scanf("%s", name);

            int pos = findSpecies(name);

            if (pos == -1) {
                strcpy(a[m].name, name);
                a[m].count = 1;
                m++;
            } else {
                a[pos].count++;
            }
        }

        else if (type == 'R') {
            Species temp[MAXN];
            int j;
            int limit;

            /* Copy current species */
            for (j = 0; j < m; j++) {
                temp[j] = a[j];
            }

            /* Sort by frequency, then name */
            qsort(temp, m, sizeof(Species), cmp);

            limit = (m < k) ? m : k;

            for (j = 0; j < limit; j++) {
                if (j > 0)
                    printf(" ");

                printf("%s", temp[j].name);
            }

            printf("\n");
        }
    }

    return 0;
}
