/*
  Ring Buffer:
  retain one element as sentinal to differ empty and full.

  The size of ring buffer should be 2^n, Because X % Y == X & (Y-1) is true only when Y = 2^n.
  otherwise remainder operation will be expensive.
*/

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


queue_t *init_ring_buffer(size_t rb_size)
{
	queue_t *rb = malloc(sizeof(queue_t));
	if(!rb){
		printf("memory allocation fails\n");
		return NULL;
	}
	rb->buffer = malloc(sizeof(rb_size));
	if(!rb->buffer){
		printf("memory allocation fails\n");
		free(rb);
		return NULL;
	}
	for(size_t i = 0; i < rb_size; i++){
		rb->buffer[i] = 0;
	}
	rb->size = rb_size;
	rb->buffer_mask = rb->size-1;
	rb->head = 0;
	rb->tail = 0;

	return rb;
}


/*
  only two condition represents ring buffer is full.
  1. head index - tail index = size - 1
  2. head index - tail index = -1

  if size is 2^n, then
  for both result, the last n-1 bit is 1.  
*/
inline bool ring_buffer_is_full(queue_t *q)
{
	return (q->head - q->tail & RING_BUFFER_MASK(q)) == RING_BUFFER_MASK(q);
}

/*
  First, check ring buffer is enough or not.
  then copy the data to ring buffer.
*/
bool enqueue(queue_t *q, uint8_t *data, size_t size)
{
	if(q->head == q->tail){
		if (size > q->size-1){
			printf("ring buffer not enough\n");
			return false;
		}
	}
	else {
		if (((q->tail - q->head)&RING_BUFFER_MASK(q)-1) < size){
			printf("ring buffer not enough\n");
			return false;
		}
	}
	for (size_t i = 0; i < size; i++){
		memcpy(&q->buffer[q->head], &data[i], 1);
		q->head++;
		q->head = q->head & RING_BUFFER_MASK(q);
	}
	return true;
}

bool dequeue(queue_t *q, uint8_t *dest, size_t size)
{
	if (((q->head - q->tail)&RING_BUFFER_MASK(q)) < size){
		printf("ring buffer not enough data\n");
		return false;
	}
	for(size_t i = 0; i < size; i++){
		memcpy(&dest[i], &q->buffer[q->tail+i], 1);
		q->buffer[q->tail] = 0;
		q->tail++;
		q->tail = q->tail & RING_BUFFER_MASK(q);
	}
	return true;
}

bool print_ring_buffer(queue_t *q)
{
	if(!q){
		printf("error, Null ring buffer\n");
		return false;
	}
	printf("\n");
	uint8_t *buffer = q->buffer;
	printf("ring buffer content: \n");
	for(size_t i = 0; i < q->size; i++){
		printf("%u\n", buffer[i]);
	}
	printf("\nhead index: %ld, tail index: %ld\n", q->head, q->tail);
	return true;
}
