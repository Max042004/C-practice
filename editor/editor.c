/*
 * Editor
 * 
 * Read data from a file and print in terminal.
 * 
 * Edit mode:
 * Receive keyboard input to move the cursor and modify the word where cursor at.
 * Cursor should have color.
 *
 * Write the modified data back to file.
 *
 * Issues:
 * 1. empty file -> list head point to itself, container_of become error 
 */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include <stdbool.h>

#define MAX_SIZE 10000
#define DEFAULT_LINE_SIZE 100

// To insert words, should using linked list. If using array, needs rearranging everytime for any insert, cost O(N).
struct line {
	struct list_head list;
	char *data;
	unsigned word_num, line_num;
};

typedef struct {
	unsigned column;
	unsigned row;
	unsigned column_char_index;
	struct list_head *cur_line;
} cursor;

struct line *init_lines(unsigned line_num)
{
	struct line *l;
	l = malloc(sizeof(struct line));
	if (!l){
		printf("malloc fail\n");
		return NULL;
	}
	l->data = malloc(DEFAULT_LINE_SIZE);
	if (!l->data) {
		printf("malloc fail\n");
		free(l);
		return NULL;
	}
	l->word_num = 0;
	l->line_num = line_num;
	return l;
}

void free_data(struct list_head *head)
{
	struct line *entry, *safe;
	list_for_each_entry_safe(entry, safe, head, list){
		free(entry->data);
		free(entry);
	}
	free(head);
	return;
}

/* need error handling?
 *
 * cursor point a data content, not directly point to screen
 *
 * maybe don't need total_line_num?
 */
bool cursor_pos(struct list_head *head, int key, cursor *curs, unsigned *total_line, FILE *log)
{	
	switch(key) {
		case KEY_UP:
			if (curs->row > 0){
				curs->row--;
				curs->cur_line = curs->cur_line->prev;
				struct line *new_line = container_of(curs->cur_line, struct line, list);
				if (curs->column_char_index > strlen(new_line->data)-1) {
					curs->column_char_index = strlen(new_line->data)-1;
					if (new_line->data[curs->column_char_index]) == '\t') curs->column = curs->column_char_index+7;
					else curs->column = curs->column_char_index;
				}
			}
			break;
		case KEY_DOWN:
			if (curs->row < (*total_line-1)){
				curs->cur_line = curs->cur_line->next;
				curs->row++;
				struct line *new_line = container_of(curs->cur_line, struct line, list);
				if (curs->column_char_index > strlen(new_line->data)-1) {
					curs->column_char_index = strlen(new_line->data)-1;
					if (new_line->data[curs->column_char_index]) == '\t') curs->column = curs->column_char_index+7;
					else curs->column = curs->column_char_index;
				}
			}
			break;
		case KEY_LEFT:
			if (curs->column > 0) {
				curs->column_char_index--;
				curs->column--;
				if (((container_of(curs->cur_line, struct line, list))->data[curs->column_char_index]) == '\t') {
					curs->column -= 7;
				}
				fprintf(log, "%c\n", (container_of(curs->cur_line, struct line, list))->data[curs->column_char_index]);
				    
			}
			break;
		case KEY_RIGHT:
			if (curs->column < (container_of(curs->cur_line, struct line, list))->word_num) {
				curs->column_char_index++;
				curs->column++;
				if (((container_of(curs->cur_line, struct line, list))->data[curs->column_char_index]) == '\t') 
					curs->column += 7;
				fprintf(log, "%c\n", (container_of(curs->cur_line, struct line, list))->data[curs->column_char_index]);
			}
			break;
		default:
			break;
	}
	move(curs->row, curs->column);
	fprintf(log, "%d %d\n", curs->row, curs->column);
	return true;
}

struct list_head *read_file(char *fpath, unsigned *total_line)
{
	FILE *fptr = fopen(fpath, "r");
	if (!fptr) {
        	printf("ERROR: Cannot open file %s\n", fpath);
        	exit(1);
    	}
    	size_t i = 0;
    	int ch;
	char *data = malloc(MAX_SIZE);
    	while (i < MAX_SIZE - 1 && (ch = fgetc(fptr)) != EOF) {
        	data[i++] = ch;
	}
	data[i] = '\0';
	fclose(fptr);

	i = 0;
	unsigned alloc_size = 1;
	struct line *lines;
	struct list_head *head;
	head = malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(head);

	unsigned line_num;
	for(line_num = 0; data[i] != '\0'; line_num++) {
		lines = init_lines(line_num);
		if (!lines) return head;

		int j;
		for(j = 0; data[i] != '\n' && data[i] != '\0'; i++, j++){
			memcpy(&lines->data[j], &data[i], 1);
			if (lines->data[j] == '\t') lines->word_num += 8;
			else lines->word_num++;
			if ((j % 100) == 98) {
				lines->data = realloc(lines->data, (++alloc_size)*DEFAULT_LINE_SIZE);
			}
		}
		if (data[i] == '\n') {
			lines->data[j] = '\n';
			i++;
		}
		lines->data[++j] = '\0';
		list_add_tail(&lines->list, head);
	}
	*total_line = line_num;

	free(data);
	return head;
}

bool edit_mode(struct list_head *data_head, char *mode, cursor *curs, unsigned *total_line, FILE *log)
{
	/*mvprintw(LINES-1, 0, "INSERT");
	refresh();*/
	int ch;
	while ((ch = getch()) != 27) {
		cursor_pos(data_head, ch, curs, total_line, log);
	}
	/*mvprintw(LINES-1, 0, "%*s", COLS, " ");
	refresh();*/
	return true;
}

void print_data(struct list_head *data_head, char *mode, cursor *curs, unsigned *total_line, FILE *log)
{
	initscr();
	struct line *entry;
	list_for_each_entry(entry, data_head, list){
		printw("%s", entry->data);
	}
	refresh();

	raw();
	noecho();
	keypad(stdscr, TRUE);
	int ch;
	while ((ch = getch()) != 'q') {
		if (!cursor_pos(data_head, ch, curs, total_line, log)) break;
		if (ch == 'i') {
			*mode = 'i';
			/*if (edit_mode(data_head, mode, curs, total_line)){
				*mode = 'n';
			};*/
		}	
	}
	return;
}

bool close_editor()
{
	return endwin();
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("ERROR, please provide file path\n");
		return 0;
	}

	// tmp for debug
	FILE *log = fopen("./log.txt", "w");

	char mode;
	mode = 'n';
	char *file_path = argv[1];
	
	unsigned total_line = 0;
	struct list_head *head = read_file(file_path, &total_line);
	
	// The top-leftest position (0, 0)
	cursor curs = {.column = 0, .row = 0, .column_char_index = 0,.cur_line = head->next};
	
	print_data(head, &mode, &curs, &total_line, log);
	close_editor();
	free_data(head);
	fclose(log);

	return 0;
}
