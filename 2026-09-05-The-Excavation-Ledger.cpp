#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, k;
    scanf("%d %d", &n, &k);

    /*
        freq[r] = number of previous prefix sums
                  having remainder r modulo k.
    */
    long long *freq = (long long *)calloc(k, sizeof(long long));

    long long answer = 0;
    long long prefix = 0;

    /*
        Empty prefix has sum 0,
        so remainder 0 has appeared once.
    */
    freq[0] = 1;

    for (int i = 0; i < n; i++) {
        long long x;
        scanf("%lld", &x);

        prefix += x;

        /*
            C's % can be negative.
            Normalize remainder into [0, k-1].
        */
        long long rem = prefix % k;

        if (rem < 0)
            rem += k;

        /*
            Every previous prefix with the same remainder
            creates a subarray whose sum is divisible by k.
        */
        answer += freq[rem];

        freq[rem]++;
    }

    printf("%lld\n", answer);

    free(freq);

    return 0;
}
