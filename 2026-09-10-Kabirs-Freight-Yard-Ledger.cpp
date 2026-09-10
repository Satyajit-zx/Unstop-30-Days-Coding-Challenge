#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

typedef struct {
    ll d, sum;
    int first;
} Node;

int cmpDest(const void *a, const void *b) {
    Node *x = (Node*)a, *y = (Node*)b;
    if (x->d < y->d) return -1;
    if (x->d > y->d) return 1;
    return x->first - y->first;
}

int cmpFirst(const void *a, const void *b) {
    return ((Node*)a)->first - ((Node*)b)->first;
}

int main() {
    int n;
    scanf("%d", &n);

    ll *w = malloc(n * sizeof(ll));
    ll *d = malloc(n * sizeof(ll));
    ll *gap = calloc(n, sizeof(ll));
    int *st = malloc(n * sizeof(int));
    Node *a = malloc(n * sizeof(Node));

    for (int i = 0; i < n; i++)
        scanf("%lld %lld", &w[i], &d[i]);

    /* Next greater element */
    int top = 0;

    for (int i = n - 1; i >= 0; i--) {
        while (top && w[st[top - 1]] <= w[i])
            top--;

        if (top)
            gap[i] = st[top - 1] - i;

        st[top++] = i;
    }

    /* Sort by destination */
    for (int i = 0; i < n; i++) {
        a[i].d = d[i];
        a[i].sum = gap[i];
        a[i].first = i;
    }

    qsort(a, n, sizeof(Node), cmpDest);

    /* Merge equal destinations */
    int k = 0;

    for (int i = 0; i < n; ) {
        int j = i + 1;
        a[k] = a[i];

        while (j < n && a[j].d == a[i].d) {
            a[k].sum += a[j].sum;
            j++;
        }

        k++;
        i = j;
    }

    /* First appearance order */
    qsort(a, k, sizeof(Node), cmpFirst);

    for (int i = 0; i < k; i++)
        printf("%lld %lld\n", a[i].d, a[i].sum);

    free(w);
    free(d);
    free(gap);
    free(st);
    free(a);

    return 0;
}

