/* Text Editor
 *
 * input:
 * initial string
 * M- the number of operations
 *
 * output:
 * final string
 *
 * Left | Gap | Right
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_SIZE 65536
#define GAP_GROW_SIZE 4096

typedef struct {
	char *buffer, *ebuffer, *gap, *egap;
	size_t size;
} gap_buffer_t;
 
enum editor_key {
          ARROW_LEFT = 0x3e8, ARROW_RIGHT,
};
  
struct {
          int cursor_x;
} ec = {
          .cursor_x = 0
};

gap_buffer_t *gap_init(size_t initial_size)
{
	gap_buffer_t *gb = malloc(sizeof(gap_buffer_t));
	if(!gb) return NULL;

	if(!(gb->buffer = malloc(initial_size))) {
		free(gb);
		return NULL;
	}
	
	gb->size = initial_size;
	gb->ebuffer = gb->buffer+initial_size;
	gb->gap = gb->buffer;
	gb->egap = gb->ebuffer;

	return gb;
}

static inline size_t gap_length(gap_buffer_t *gb)
{
	return (gb->gap-gb->buffer)+(gb->ebuffer-gb->egap);
}

static char *gap_ptr(gap_buffer_t *gb, size_t pos)
{
	size_t front_size = gb->gap - gb->buffer;
	if (pos <= front_size) return gb->buffer + pos;
	return gb->egap + (pos-front_size);
}

static void gap_move(gap_buffer_t *gb, size_t pos)
{
	char *dest = gap_ptr(gb, pos);
	
	if (dest < gb->gap) {
		size_t len = gb->gap - dest;
		gb->gap -= len;
		gb->egap -=len;
		memmove(gb->egap, gb->gap, len);
	}
	else if (dest > gb->egap) {
		size_t len = dest - gb->egap;
		memmove(gb->gap, gb->egap, len);
		gb->gap += len;
		gb->egap += len;
	}
}

static bool gap_grow(gap_buffer_t *gb, size_t min_gap)
{
	size_t gap_size = gb->egap - gb->gap;

	if (gap_size > min_gap) return true;

	size_t text_size = gap_length(gb);
	size_t new_size = text_size + min_gap + GAP_GROW_SIZE;

	size_t gap_pos = gb->gap - gb->buffer;
	size_t after_gap_size = gb->ebuffer - gb->egap;

	char *new_buffer = realloc(gb->buffer, new_size);
	if (!new_buffer) return false;
	
	gb->buffer = new_buffer;
	gb->gap = gb->buffer + gap_pos;
	gb->ebuffer = gb->buffer + new_size;
	gb->egap = gb->ebuffer - after_gap_size;

	if (after_gap_size > 0) memmove(gb->egap, gb->gap+gap_size, after_gap_size);
	
	gb->size = new_size;
	return true;
}

static bool gap_insert(gap_buffer_t *gb, size_t pos, const char *text, size_t len)
{
	gap_move(gb, pos);

	if (!gap_grow(gb, len)) return false;

	memcpy(gb->gap, text, len);
	gb->gap += len;

	return true;
}

static void gap_delete(gap_buffer_t *gb, size_t pos, size_t len)
{
	gap_move(gb, pos);

	size_t available = gb->ebuffer - gb->egap;
	if (len > available) len = available;
	gb->egap += len;
}

static void gap_load(gap_buffer_t *gb)
{
	char *str = malloc(256);
	scanf("%s\n", str);
	gap_insert(gb, gap_length(gb), str, strlen(str));
	ec.cursor_x += strlen(str);
       	free(str);	
}

static void editor_move_cursor(gap_buffer_t *gb, int key)
{
	switch(key){
	case 'L':
		if(ec.cursor_x != 0) {
			ec.cursor_x--;
		}
		break;

	case 'R':
		if(ec.cursor_x < gap_length(gb)) {
			ec.cursor_x++;
		}
	}
	gap_move(gb, ec.cursor_x);
}

int main()
{
	gap_buffer_t *gb = gap_init(INITIAL_SIZE);
	gap_load(gb);
	int m;
	char op, in_c;
	scanf("%d", &m);
	for(int i = 0; i < m; i++) {
		int c;
		while((c = getchar()) != '\n' && c != EOF);
		scanf("%c", &op);

		switch(op){
			case 'L':
				editor_move_cursor(gb, op);
				break;
			case 'R':
				editor_move_cursor(gb, op);
				break;
			case 'D':
				if (ec.cursor_x == 0) break;
				gap_delete(gb, --ec.cursor_x, 1);
				break;
			case 'I':
				scanf(" %c", &in_c);
				gap_insert(gb, ec.cursor_x, &in_c, 1);
				ec.cursor_x++;
				break;
		}
	}

	size_t front_size = gb->gap - gb->buffer;
	size_t back_size = gb->ebuffer - gb->egap;
	size_t gap_size = gb->egap - gb->gap;
	char *front = malloc(front_size+1), *back = malloc(back_size+1);
	memcpy(front, gb->buffer, front_size);
	front[front_size] = '\0';
	memcpy(back, gb->egap, back_size);
	back[back_size] = '\0';

	printf("%s%s", front, back);

	free(gb);
	free(front);
	free(back);
}

