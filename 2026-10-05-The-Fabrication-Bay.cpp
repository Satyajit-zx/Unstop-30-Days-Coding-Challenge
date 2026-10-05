#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long profit;
    long long deadline;
} Order;

// Minimum Heap Implementation
typedef struct {
    long long *data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createHeap(int capacity) {
    MinHeap* hp = (MinHeap*)malloc(sizeof(MinHeap));
    hp->data = (long long*)malloc(sizeof(long long) * (capacity + 1));
    hp->size = 0;
    hp->capacity = capacity;
    return hp;
}

void swap(long long *a, long long *b) {
    long long temp = *a;
    *a = *b;
    *b = temp;
}

void push(MinHeap *hp, long long val) {
    hp->size++;
    hp->data[hp->size] = val;
    int i = hp->size;
    while (i > 1 && hp->data[i] < hp->data[i / 2]) {
        swap(&hp->data[i], &hp->data[i / 2]);
        i /= 2;
    }
}

long long pop(MinHeap *hp) {
    long long top = hp->data[1];
    hp->data[1] = hp->data[hp->size];
    hp->size--;
    
    int i = 1;
    while (2 * i <= hp->size) {
        int left = 2 * i;
        int right = 2 * i + 1;
        int smallest = i;

        if (left <= hp->size && hp->data[left] < hp->data[smallest])
            smallest = left;
        if (right <= hp->size && hp->data[right] < hp->data[smallest])
            smallest = right;

        if (smallest != i) {
            swap(&hp->data[i], &hp->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

// Comparator function to sort orders by deadline ascending
int compareOrders(const void *a, const void *b) {
    Order *o1 = (Order *)a;
    Order *o2 = (Order *)b;
    if (o1->deadline < o2->deadline) return -1;
    if (o1->deadline > o2->deadline) return 1;
    return 0;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    Order *orders = (Order *)malloc(sizeof(Order) * n);
    for (int i = 0; i < n; i++) {
        scanf("%lld %lld", &orders[i].profit, &orders[i].deadline);
    }

    // Deadline ke base par sort
    qsort(orders, n, sizeof(Order), compareOrders);

    MinHeap *hp = createHeap(n);

    for (int i = 0; i < n; i++) {
        push(hp, orders[i].profit);
        // Agar heap size, deadline se bada ho jaye toh min profit order pop karo
        if (hp->size > orders[i].deadline) {
            pop(hp);
        }
    }

    long long total_profit = 0;
    int count = hp->size;

    while (hp->size > 0) {
        total_profit += pop(hp);
    }

    printf("%lld %d\n", total_profit, count);

    free(orders);
    free(hp->data);
    free(hp);

    return 0;
}
