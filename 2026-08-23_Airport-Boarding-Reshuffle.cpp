#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    list<int> q;
    int passenger = 1;

    while (m--) {
        char ch;
        cin >> ch;

        if (ch == 'A') {
            // Next passenger arrives
            q.push_back(passenger);
            passenger++;
        }

        else if (ch == 'P') {
            int x;
            cin >> x;

            // Find passenger x
            auto it = find(q.begin(), q.end(), x);

            // If x is present, move it to front
            if (it != q.end()) {
                q.splice(q.begin(), q, it);
            }
        }

        else if (ch == 'B') {
            // If queue is empty
            if (q.empty()) {
                cout << 0 << endl;
            }
            else {
                cout << q.front() << endl;
                q.pop_front();
            }
        }
    }

    return 0;
}
