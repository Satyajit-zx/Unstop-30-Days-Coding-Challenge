#include <stdio.h>
#include <stdlib.h>

typedef unsigned int U;

typedef struct {
    int ch[2], cnt;
} Node;

Node *trie;
int nodes = 1;

void add(U x, int v) {
    int p = 0;
    trie[p].cnt += v;

    for (int b = 30; b >= 0; b--) {
        int bit = (x >> b) & 1;

        if (!trie[p].ch[bit])
            trie[p].ch[bit] = nodes++;

        p = trie[p].ch[bit];
        trie[p].cnt += v;
    }
}

U query(U x) {
    int p = 0;
    U ans = 0;

    for (int b = 30; b >= 0; b--) {
        int bit = (x >> b) & 1;
        int q = trie[p].ch[bit ^ 1];

        if (q && trie[q].cnt > 0) {
            ans |= (1U << b);
            p = q;
        } else {
            p = trie[p].ch[bit];
        }
    }

    return ans;
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);

    U *p = malloc((n + 1) * sizeof(U));

    p[0] = 0;
    for (int i = 1; i <= n; i++) {
        U x;
        scanf("%u", &x);
        p[i] = p[i - 1] ^ x;
    }

    trie = calloc((n + 1) * 31, sizeof(Node));

    U ans = 0;

    for (int r = 1; r <= n; r++) {
        /* P[r-1] is now allowed */
        add(p[r - 1], 1);

        /* Remove prefix that makes length > K */
        if (r - k - 1 >= 0)
            add(p[r - k - 1], -1);

        U cur = query(p[r]);
        if (cur > ans)
            ans = cur;
    }

    printf("%u\n", ans);

    free(p);
    free(trie);

    return 0;
}

