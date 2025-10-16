#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define RING_BUFFER_MASK(rb) rb->buffer_mask

typedef struct {
    uint8_t *buffer;
    size_t size;
    size_t buffer_mask;

    size_t head, tail;
} queue_t;

queue_t *init_ring_buffer(size_t rb_size);

bool enqueue(queue_t *q, uint8_t *data, size_t size);

bool dequeue(queue_t *q, uint8_t *dest, size_t size);

bool print_ring_buffer(queue_t *q);

#endif // RING_BUFFER_H
