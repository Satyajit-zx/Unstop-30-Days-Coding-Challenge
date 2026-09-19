#include <stdio.h>

#define MAX_N 200005

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    char belt[MAX_N];
    scanf("%s", belt);

    int need[26] = {0};
    int have[26] = {0};

    int i;

    /* Read required component types */
    for (i = 0; i < m; i++) {
        char type;
        int count;

        scanf(" %c %d", &type, &count);
        need[type - 'A'] = count;
    }

    int requiredTypes = 0;

    for (i = 0; i < 26; i++) {
        if (need[i] > 0) {
            requiredTypes++;
        }
    }

    int satisfiedTypes = 0;
    int left = 0;
    int answer = n + 1;

    /* Sliding Window */
    for (i = 0; i < n; i++) {

        int c = belt[i] - 'A';
        have[c]++;

        if (need[c] > 0 && have[c] == need[c]) {
            satisfiedTypes++;
        }

        /* Shrink from left while all requirements are satisfied */
        while (satisfiedTypes == requiredTypes && left <= i) {

            int currentLength = i - left + 1;

            if (currentLength < answer) {
                answer = currentLength;
            }

            int leftChar = belt[left] - 'A';

            if (need[leftChar] > 0 &&
                have[leftChar] == need[leftChar]) {
                satisfiedTypes--;
            }

            have[leftChar]--;
            left++;
        }
    }

    if (answer == n + 1) {
        printf("-1\n");
    } else {
        printf("%d\n", answer);
    }

    return 0;
}
