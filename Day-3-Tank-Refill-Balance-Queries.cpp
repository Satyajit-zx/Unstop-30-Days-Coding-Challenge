#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<long long> prefix(N + 1, 0);

    for (int i = 1; i <= N; i++) {
        long long x;
        cin >> x;
        prefix[i] = prefix[i - 1] + x;
    }

    int Q;
    cin >> Q;

    while (Q--) {
        int L, R;
        cin >> L >> R;

        long long sum = prefix[R] - prefix[L - 1];

        cout << sum << " ";

        if (sum > 0)
            cout << "SURPLUS";
        else if (sum < 0)
            cout << "DEFICIT";
        else
            cout << "BALANCED";

        cout << '\n';
    }

    return 0;
}
