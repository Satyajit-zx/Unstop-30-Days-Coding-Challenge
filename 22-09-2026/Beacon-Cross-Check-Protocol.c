#include <stdio.h>
#include <stdlib.h>

#define MAXQ 200005
#define MAXBITS 20
#define MAXNODE (MAXQ * 21)

typedef struct {
    int child[2];
    int cnt;
} TrieNode;

TrieNode trie[MAXNODE];
int nodes = 1;

int codeOf[MAXQ];
int active[MAXQ];

/* Each code can have multiple beacon IDs.
   Linked-list style structure is avoided by using a
   simple sorted dynamic array per code. */

int *ids[1 << 20];
int sz[1 << 20];
int cap[1 << 20];

void add_id(int code, int id) {
    if (sz[code] == cap[code]) {
        cap[code] = cap[code] ? cap[code] * 2 : 2;
        ids[code] = (int *)realloc(ids[code],
                                   cap[code] * sizeof(int));
    }

    int p = sz[code];

    while (p > 0 && ids[code][p - 1] > id) {
        ids[code][p] = ids[code][p - 1];
        p--;
    }

    ids[code][p] = id;
    sz[code]++;
}

void remove_id(int code, int id) {
    int l = 0, r = sz[code] - 1;

    while (l <= r) {
        int mid = (l + r) / 2;

        if (ids[code][mid] == id) {
            for (int i = mid; i + 1 < sz[code]; i++) {
                ids[code][i] = ids[code][i + 1];
            }
            sz[code]--;
            return;
        }

        if (ids[code][mid] < id)
            l = mid + 1;
        else
            r = mid - 1;
    }
}

void trie_insert(int x) {
    int node = 0;
    trie[node].cnt++;

    for (int b = MAXBITS; b >= 0; b--) {
        int bit = (x >> b) & 1;

        if (trie[node].child[bit] == 0) {
            trie[node].child[bit] = nodes++;
        }

        node = trie[node].child[bit];
        trie[node].cnt++;
    }
}

void trie_remove(int x) {
    int node = 0;
    trie[node].cnt--;

    for (int b = MAXBITS; b >= 0; b--) {
        int bit = (x >> b) & 1;
        node = trie[node].child[bit];
        trie[node].cnt--;
    }
}

/* Returns the code which gives maximum XOR with x */
int get_max_xor_code(int x) {
    int node = 0;
    int result = 0;

    for (int b = MAXBITS; b >= 0; b--) {
        int bit = (x >> b) & 1;
        int wanted = bit ^ 1;

        if (trie[node].child[wanted] != 0 &&
            trie[trie[node].child[wanted]].cnt > 0) {

            result |= (wanted << b);
            node = trie[node].child[wanted];

        } else {
            node = trie[node].child[bit];
            result |= (bit << b);
        }
    }

    return result;
}

int main() {
    int M;
    scanf("%d", &M);

    char op[10];

    for (int q = 0; q < M; q++) {
        int id, code;

        scanf("%s", op);

        if (op[0] == 'O' && op[1] == 'N') {

            scanf("%d %d", &id, &code);

            /* If already active, remove old code first */
            if (active[id]) {
                trie_remove(codeOf[id]);
                remove_id(codeOf[id], id);
            }

            codeOf[id] = code;
            active[id] = 1;

            trie_insert(code);
            add_id(code, id);

        } 
        else if (op[0] == 'O' && op[1] == 'F') {

            scanf("%d", &id);

            if (active[id]) {
                trie_remove(codeOf[id]);
                remove_id(codeOf[id], id);
                active[id] = 0;
            }

        } 
        else if (op[0] == 'C') {

            scanf("%d", &code);

            int bestCode = get_max_xor_code(code);
            int bestValue = bestCode ^ code;

            /* Smallest station ID for this code */
            int bestId = ids[bestCode][0];

            printf("%d %d\n", bestValue, bestId);
        }
    }

    return 0;
}
