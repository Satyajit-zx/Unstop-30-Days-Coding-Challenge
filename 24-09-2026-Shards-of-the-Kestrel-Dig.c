#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 200005
#define HASH_SIZE (1 << 20)

typedef long long ll;

int trie[MAXN][26];
int nodes = 1;

int hNode[HASH_SIZE];
ll hEra[HASH_SIZE];
int hCount[HASH_SIZE];

unsigned long long hashKey(int node, ll era) {
    unsigned long long x = (unsigned long long)node;
    unsigned long long y = (unsigned long long)era;

    x ^= y + 0x9e3779b97f4a7c15ULL + (x << 6) + (x >> 2);
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;

    return x;
}

void addCount(int node, ll era) {
    unsigned long long h = hashKey(node, era);
    int pos = h & (HASH_SIZE - 1);

    while (hNode[pos] != 0) {
        if (hNode[pos] == node && hEra[pos] == era) {
            hCount[pos]++;
            return;
        }
        pos = (pos + 1) & (HASH_SIZE - 1);
    }

    hNode[pos] = node;
    hEra[pos] = era;
    hCount[pos] = 1;
}

int getCount(int node, ll era) {
    unsigned long long h = hashKey(node, era);
    int pos = h & (HASH_SIZE - 1);

    while (hNode[pos] != 0) {
        if (hNode[pos] == node && hEra[pos] == era)
            return hCount[pos];

        pos = (pos + 1) & (HASH_SIZE - 1);
    }

    return 0;
}

int insertCode(char *s) {
    int cur = 0;

    for (int i = 0; s[i]; i++) {
        int c = s[i] - 'a';

        if (trie[cur][c] == 0) {
            trie[cur][c] = nodes++;
        }

        cur = trie[cur][c];
    }

    return cur;
}

int findPrefix(char *s) {
    int cur = 0;

    for (int i = 0; s[i]; i++) {
        int c = s[i] - 'a';

        if (trie[cur][c] == 0)
            return -1;

        cur = trie[cur][c];
    }

    return cur;
}

int main() {
    int Q;
    scanf("%d", &Q);

    char op[10];
    char str[200005];
    ll era;

    while (Q--) {
        scanf("%s", op);

        if (strcmp(op, "ADD") == 0) {
            scanf("%s %lld", str, &era);

            int cur = 0;

            for (int i = 0; str[i]; i++) {
                int c = str[i] - 'a';

                if (trie[cur][c] == 0)
                    trie[cur][c] = nodes++;

                cur = trie[cur][c];

                addCount(cur, era);
            }
        }
        else {
            scanf("%s %lld", str, &era);

            int cur = findPrefix(str);

            if (cur == -1)
                printf("0\n");
            else
                printf("%d\n", getCount(cur, era));
        }
    }

    return 0;
}
