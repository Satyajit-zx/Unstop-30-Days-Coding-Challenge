#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Attendee {
    int id;
    int count;
    int first_seen; // 0-based index of the very first '+' scan
};

bool compareAttendees(const Attendee &a, const Attendee &b) {
    if (a.count != b.count) {
        return a.count > b.count; // Higher net count first
    }
    return a.first_seen < b.first_seen; // Earlier first scan first
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    unordered_map<int, int> netCounts;
    unordered_map<int, int> firstSeen;

    for (int i = 0; i < n; i++) {
        char op;
        int id;
        cin >> op >> id;

        if (op == '+') {
            netCounts[id]++;
            // Record only the first appearance of '+'
            if (firstSeen.find(id) == firstSeen.end()) {
                firstSeen[id] = i;
            }
        } else if (op == '-') {
            netCounts[id]--;
        }
    }

    vector<Attendee> candidates;
    for (const auto &entry : netCounts) {
        int id = entry.first;
        int count = entry.second;

        // Only include attendees with positive net scan count
        if (count > 0) {
            candidates.push_back({id, count, firstSeen[id]});
        }
    }

    sort(candidates.begin(), candidates.end(), compareAttendees);

    int limit = min((int)candidates.size(), k);
    for (int i = 0; i < limit; i++) {
        cout << candidates[i].id << " " << candidates[i].count << "\n";
    }

    return 0;
}
