#include <stdio.h>
#include <stdlib.h>

#define MAXN 100000
#define MAXBIT 19

/*
    Each insertion creates at most 20 new trie nodes.
    We need a root node for each prefix as well.
*/
#define MAXNODE ((MAXN + 5) * 21)

typedef struct {
    int child[2];
    int cnt;
} Node;

Node trie[MAXNODE];
int root[MAXN + 5];
int nodes = 0;

/*
    Insert value x into the trie represented by oldRoot.
    Returns the root of the new persistent trie.
*/
int insertValue(int oldRoot, int x) {
    int newRoot = ++nodes;
    int oldNode = oldRoot;
    int newNode = newRoot;
    int bit, b;

    trie[newNode].cnt = trie[oldNode].cnt + 1;
    trie[newNode].child[0] = trie[oldNode].child[0];
    trie[newNode].child[1] = trie[oldNode].child[1];

    for (bit = MAXBIT; bit >= 0; --bit) {
        b = (x >> bit) & 1;

        oldNode = trie[oldNode].child[b];

        int nextNode = ++nodes;

        trie[nextNode].cnt = trie[oldNode].cnt + 1;
        trie[nextNode].child[0] = trie[oldNode].child[0];
        trie[nextNode].child[1] = trie[oldNode].child[1];

        trie[newNode].child[b] = nextNode;

        newNode = nextNode;
    }

    return newRoot;
}

/*
    Returns maximum value of (a[i] XOR x)
    for l <= i <= r.
*/
int query(int leftRoot, int rightRoot, int x) {
    int ans = 0;
    int leftNode = leftRoot;
    int rightNode = rightRoot;

    int bit;

    for (bit = MAXBIT; bit >= 0; --bit) {
        int xb = (x >> bit) & 1;

        /*
            To maximize XOR, we prefer the opposite bit.
        */
        int preferred = xb ^ 1;

        int leftChild = trie[leftNode].child[preferred];
        int rightChild = trie[rightNode].child[preferred];

        int countInRange =
            trie[rightChild].cnt - trie[leftChild].cnt;

        if (countInRange > 0) {
            /*
                Opposite bit exists in the query range.
                Taking it makes this XOR bit 1.
            */
            ans |= (1 << bit);

            leftNode = leftChild;
            rightNode = rightChild;
        } else {
            /*
                Opposite bit doesn't exist.
                We must take the same bit.
            */
            int other = xb;

            leftNode = trie[leftNode].child[other];
            rightNode = trie[rightNode].child[other];
        }
    }

    return ans;
}

int main() {
    int N, Q;
    int i;

    /*
        Node 0 is the empty trie.
        Its count and children are already zero.
    */
    nodes = 0;

    scanf("%d", &N);

    for (i = 1; i <= N; ++i) {
        int x;
        scanf("%d", &x);

        root[i] = insertValue(root[i - 1], x);
    }

    scanf("%d", &Q);

    while (Q--) {
        int l, r, x;
        scanf("%d %d %d", &l, &r, &x);

        printf("%d\n", query(root[l - 1], root[r], x));
    }

    return 0;
}
