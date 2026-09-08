#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;

typedef struct {
    ll m, b;
} Line;

ll value(Line l, ll x) {
    return l.m * x + l.b;
}

/* Check whether l2 is unnecessary between l1 and l3 */
int bad(Line l1, Line l2, Line l3) {
    __int128 left =
        (__int128)(l2.b - l1.b) * (l2.m - l3.m);

    __int128 right =
        (__int128)(l3.b - l2.b) * (l1.m - l2.m);

    return left >= right;
}

int main() {
    int n;
    ll w;

    scanf("%d %lld", &n, &w);

    ll *energy = (ll *)malloc((n + 1) * sizeof(ll));
    ll *dp = (ll *)malloc((n + 1) * sizeof(ll));
    Line *hull = (Line *)malloc((n + 1) * sizeof(Line));

    for (int i = 1; i <= n; i++)
        scanf("%lld", &energy[i]);

    int head = 0, tail = 0;

    /* Night 1 must be active */
    dp[1] = energy[1];

    /* Line corresponding to j = 1 */
    hull[0].m = -2LL * w;
    hull[0].b = dp[1] + w;

    for (int i = 2; i <= n; i++) {

        /* Remove lines that are worse for current x=i */
        while (head < tail &&
               value(hull[head], i) >= value(hull[head + 1], i)) {
            head++;
        }

        /* Calculate dp[i] */
        dp[i] = energy[i]
              + w * (ll)i * i
              + value(hull[head], i);

        /* Add line corresponding to j=i */
        Line nl;
        nl.m = -2LL * w * i;
        nl.b = dp[i] + w * (ll)i * i;

        /* Remove redundant lines */
        while (tail > head &&
               bad(hull[tail - 1], hull[tail], nl)) {
            tail--;
        }

        hull[++tail] = nl;
    }

    printf("%lld\n", dp[n]);

    free(energy);
    free(dp);
    free(hull);

    return 0;
}

