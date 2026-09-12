#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 36

typedef struct {
    int child[ALPHA];
    int end;
} Node;

Node *trie;
int nodes = 1;

int id(char c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    return c - '0' + 26;
}

int main() {
    int n, i;
    long long total = 0;

    scanf("%d", &n);

    /* Maximum nodes = total length + root */
    trie = calloc(200005, sizeof(Node));

    for (i = 0; i < n; i++) {
        char s[25];
        int p = 0;
        int j, len, longest = 0;
        int duplicate = 0;

        scanf("%s", s);
        len = strlen(s);

        /* Find longest registered prefix */
        for (j = 0; j < len; j++) {
            int c = id(s[j]);

            if (trie[p].child[c] == 0)
                break;

            p = trie[p].child[c];

            if (trie[p].end)
                longest = j + 1;
        }

        /* Entire string already exists */
        if (j == len && trie[p].end) {
            duplicate = 1;
        }

        if (duplicate) {
            printf("-1\n");
            continue;
        }

        /* Novelty score */
        printf("%d\n", len - longest);
        total += len - longest;

        /* Insert the new string */
        p = 0;

        for (j = 0; j < len; j++) {
            int c = id(s[j]);

            if (trie[p].child[c] == 0) {
                trie[p].child[c] = nodes++;
            }

            p = trie[p].child[c];
        }

        trie[p].end = 1;
    }

    printf("Total: %lld\n", total);

    free(trie);
    return 0;
}

