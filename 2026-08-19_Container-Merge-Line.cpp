#include <stdio.h>
#include <stdlib.h>

void process_containers(int n, int* weights, int* m, long long** final_weights) {
    long long* stack = (long long*)malloc(n * sizeof(long long));
    int top = -1;

    for (int i = 0; i < n; i++) {
        long long curr = weights[i];
        
        // Push and check for cascading merges
        while (top >= 0 && stack[top] == curr) {
            curr += stack[top];
            top--;
        }
        top++;
        stack[top] = curr;
    }

    *m = top + 1;
    *final_weights = (long long*)malloc((*m) * sizeof(long long));
    for (int i = 0; i < *m; i++) {
        (*final_weights)[i] = stack[i];
    }

    free(stack);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int* weights = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &weights[i]);
    }

    int m;
    long long* final_weights;
    process_containers(n, weights, &m, &final_weights);

    printf("%d\n", m);
    for (int i = 0; i < m; i++) {
        printf("%lld ", final_weights[i]);
    }
    printf("\n");

    free(weights);
    free(final_weights);
    return 0;
}
