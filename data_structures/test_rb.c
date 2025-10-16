#include "ring_buffer.h"

int main()
{
    // Test 1: Basic enqueue and dequeue without wrap
    printf("Test 1: Basic enqueue and dequeue without wrap\n");
    queue_t *q = init_ring_buffer(16);
    if (!q) return 1;

    uint8_t data[20];
    for (uint8_t i = 0; i < 20; i++) data[i] = i + 1;

    uint8_t dest[20];

    bool res = enqueue(q, data, 5);
    if (!res) {
        printf("Enqueue 5 failed unexpectedly\n");
    } else {
        printf("Enqueue 5 succeeded\n");
    }
    print_ring_buffer(q);

    res = dequeue(q, dest, 3);
    if (!res) {
        printf("Dequeue 3 failed unexpectedly\n");
    } else {
        printf("Dequeue 3 succeeded\n");
        printf("Dequeued values: ");
        for (size_t i = 0; i < 3; i++) {
            printf("%u ", dest[i]);
        }
        printf("\n");
        if (dest[0] != 1 || dest[1] != 2 || dest[2] != 3) {
            printf("Dequeued values mismatch expected [1,2,3]\n");
        } else {
            printf("Dequeued values match expected\n");
        }
    }
    print_ring_buffer(q);

    // Test 2: Enqueue to full without wrap
    printf("\nTest 2: Enqueue to full without wrap\n");
    res = enqueue(q, data + 3, 13);
    if (!res) {
        printf("Enqueue 13 failed unexpectedly\n");
    } else {
        printf("Enqueue 13 succeeded\n");
    }
    print_ring_buffer(q);

    res = enqueue(q, data, 1);
    if (res) {
        printf("Enqueue 1 succeeded unexpectedly\n");
    } else {
        printf("Enqueue 1 failed as expected\n");
    }
    print_ring_buffer(q);

    // Test 3: Dequeue after full
    printf("\nTest 3: Dequeue after full\n");
    res = dequeue(q, dest, 15);
    if (!res) {
        printf("Dequeue 10 failed unexpectedly\n");
    } else {
        printf("Dequeue 15 succeeded\n");
        printf("Dequeued values: ");
        for (size_t i = 0; i < 15; i++) {
            printf("%u ", dest[i]);
        }
        printf("\n");
    }
    print_ring_buffer(q);

    // Cleanup for this test
    free(q->buffer);
    free(q);

    // Test 4: Wrap around test
    printf("\nTest 4: Wrap around test\n");
    q = init_ring_buffer(16);
    if (!q) return 1;

    // Enqueue 16
    res = enqueue(q, data, 16);
    if (!res) {
        printf("Enqueue 16 failed expectedly\n");
    } else {
        printf("Enqueue 16 succeeded unexpectedly\n");
    }
    print_ring_buffer(q);

    // Dequeue 10 to empty
    res = dequeue(q, dest, 10);
    if (!res) {
        printf("Dequeue 10 failed expectedly\n");
    } else {
        printf("Dequeue 10 succeeded unexpectedly\n");
        printf("Dequeued values: ");
        for (size_t i = 0; i < 10; i++) {
            printf("%u ", dest[i]);
        }
        printf("\n");
        bool match = true;
        for (size_t i = 0; i < 10; i++) {
            if (dest[i] != i + 1) match = false;
        }
        if (match) {
            printf("Dequeued values match expected\n");
        } else {
            printf("Dequeued values mismatch expected\n");
        }
    }
    print_ring_buffer(q);

    // Enqueue 15 to full with wrap
    res = enqueue(q, data, 15);
    if (!res) {
        printf("Enqueue 15 failed unexpectedly\n");
    } else {
        printf("Enqueue 15 succeeded\n");
    }
    print_ring_buffer(q);

    // Dequeue 5, now wrapped
    res = dequeue(q, dest, 5);
    if (!res) {
        printf("Dequeue 5 failed unexpectedly\n");
    } else {
        printf("Dequeue 5 succeeded\n");
        printf("Dequeued values: ");
        for (size_t i = 0; i < 5; i++) {
            printf("%u ", dest[i]);
        }
        printf("\n");
    }
    print_ring_buffer(q);

    // Enqueue 5 more to full again
    res = enqueue(q, data + 15, 5);
    if (!res) {
        printf("Enqueue 5 failed unexpectedly\n");
    } else {
        printf("Enqueue 5 succeeded\n");
    }
    print_ring_buffer(q);

    // Final dequeue all
    res = dequeue(q, dest, 15);
    if (!res) {
        printf("Dequeue 15 failed unexpectedly\n");
    } else {
        printf("Dequeue 15 succeeded\n");
        printf("Dequeued values: ");
        for (size_t i = 0; i < 15; i++) {
            printf("%u ", dest[i]);
        }
        printf("\n");
    }
    print_ring_buffer(q);

    // Test 5: Empty dequeue
    printf("\nTest 5: Empty dequeue\n");
    res = dequeue(q, dest, 1);
    if (res) {
        printf("Dequeue 1 on empty succeeded unexpectedly\n");
    } else {
        printf("Dequeue 1 on empty failed as expected\n");
    }

    // Test 6: Enqueue 0 size
    printf("\nTest 6: Enqueue 0 size\n");
    res = enqueue(q, data, 0);
    if (!res) {
        printf("Enqueue 0 failed unexpectedly\n");
    } else {
        printf("Enqueue 0 succeeded\n");
    }

    // Cleanup
    free(q->buffer);
    free(q);

    return 0;
}
