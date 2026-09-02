#include <stdio.h>

void computeHorizon(int height[], int n, int result[]) {
    int stack[n];
    int top = -1;

    for (int i = n - 1; i >= 0; i--) {
        // Pop elements that are not strictly taller than the current tower
        while (top >= 0 && height[stack[top]] <= height[i]) {
            top--;
        }

        // Calculate horizon score
        if (top == -1) {
            // No taller tower to the right; distance to reserve boundary
            result[i] = (n - 1) - i;
        } else {
            // Distance to the first strictly taller tower
            result[i] = stack[top] - i;
        }

        // Push current index onto the stack
        stack[++top] = i;
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    
    int height[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &height[i]);
    }

    int result[n];
    computeHorizon(height, n, result);

    for (int i = 0; i < n; i++) {
        printf("%d", result[i]);
        if (i != n - 1) {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}
