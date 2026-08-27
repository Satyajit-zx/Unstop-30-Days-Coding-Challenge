#include <iostream>
#include <vector>

struct TrieNode {
    int next[2];
    int count;
    TrieNode() : count(0) {
        next[0] = 0;
        next[1] = 0;
    }
};

const int MAX_BITS = 30; // 10^9 < 2^30
std::vector<TrieNode> trie;

// Inserts a number into the persistent trie and returns the index of the new root
int insert(int prev_root, int val) {
    int cur_root = trie.size();
    trie.push_back(trie[prev_root]);
    trie[cur_root].count++;
    
    int curr = cur_root;
    for (int b = MAX_BITS - 1; b >= 0; --b) {
        int bit = (val >> b) & 1;
        int prev_child = trie[curr].next[bit];
        
        int new_child = trie.size();
        trie.push_back(trie[prev_child]);
        trie[new_child].count++;
        
        trie[curr].next[bit] = new_child;
        curr = new_child;
    }
    return cur_root;
}

// Queries the maximum XOR with x in the range [l, r]
int query_max_xor(int l_root, int r_root, int x) {
    int u = l_root;
    int v = r_root;
    int max_xor = 0;

    for (int b = MAX_BITS - 1; b >= 0; --b) {
        int bit = (x >> b) & 1;
        int desired = 1 - bit;

        int count_desired = trie[trie[v].next[desired]].count - trie[trie[u].next[desired]].count;

        if (count_desired > 0) {
            max_xor |= (1 << b);
            v = trie[v].next[desired];
            u = trie[u].next[desired];
        } else {
            v = trie[v].next[bit];
            u = trie[u].next[bit];
        }
    }
    return max_xor;
}

void user_logic(int n, std::vector<int>& codes, int q, std::vector<std::vector<int>>& queries, std::vector<int>& results) {
    trie.clear();
    trie.reserve((n + 1) * (MAX_BITS + 1) + 5);
    trie.emplace_back(); // Node 0 represents the empty trie

    std::vector<int> roots(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        roots[i + 1] = insert(roots[i], codes[i]);
    }

    results.resize(q);
    for (int i = 0; i < q; ++i) {
        int l = queries[i][0];
        int r = queries[i][1];
        int x = queries[i][2];
        
        results[i] = query_max_xor(roots[l - 1], roots[r], x);
    }
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<int> codes(n);
    for (int i = 0; i < n; i++) {
        std::cin >> codes[i];
    }

    int q;
    std::cin >> q;

    std::vector<std::vector<int>> queries(q, std::vector<int>(3));
    for (int i = 0; i < q; i++) {
        std::cin >> queries[i][0] >> queries[i][1] >> queries[i][2];
    }

    std::vector<int> results(q);
    user_logic(n, codes, q, queries, results);

    for (int i = 0; i < q; i++) {
        std::cout << results[i] << "\n";
    }

    return 0;
}
