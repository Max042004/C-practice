#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node {
	struct node *next;
	int val;
};

typedef struct node node_t;
#define MAX_NODE_NUM 200002
struct node **ID[MAX_NODE_NUM];

// insert before dest node_t
static node_t *insert(node_t **dest, int val, int id, int k)
{
	if (!*dest) return;
	node_t *new_node = malloc(sizeof(node_t));
	new_node->val = val;
	new_node->next = *dest;
	*dest = new_node;
	ID[id] = ;
	return new_node;
}

static void delete(node_t **n, int id)
{
	if (!n) return;
	node_t *prev_node = n->prev;
	prev_node->next = n->next;
	n->next->prev = prev_node;
	free(n);
	ID[id] = NULL;
}

static void move_head(node_t **n, node_t *head)
{
	if (!n) return;
	node_t *prev_node = n->prev;
        prev_node->next = n->next;
        n->next->prev = prev_node;
	n->prev = head;
	n->next = head->next;
	head->next = n;
	n->next->prev = n;
}

static void move_tail(node_t **n, node_t **tail)
{
	if (!n) return;
	node_t *prev_node = n->prev;
        prev_node->next = n->next;
        n->next->prev = prev_node;
        n->next = tail;
        n->prev = tail->prev;
       	tail->prev = n;
        n->prev->next = n;
}

static void id_init()
{
	for(int i = 0; i < MAX_NODE_NUM; i++) {
		ID[i] = NULL;
	}
}

static void list_init(node_t **head, node_t **tail)
{
	*head = malloc(sizeof(node_t));
	*tail = malloc(sizeof(node_t));
	(*head)->prev = NULL;
	(*head)->next = *tail;
	(*head)->val = 0;
	(*tail)->next = NULL;
	(*tail)->prev = *head;
	(*tail)->val = 0;
}

static void list_free(node_t *head)
{
	node_t *curr, *curr_next;
	for (curr = head, curr_next = head->next; curr->next != NULL; curr = curr_next, curr_next = curr->next) {
		free(curr);
	}
	free(curr);
}

int main()
{
	int init_num, op_num, id, node_num = 0;
	scanf("%d %d", &init_num, &op_num);
	

	node_t *head, *tail;
	list_init(&head, &tail);
	id_init();
	ID[0] = &(head->next);
	int val;
	for (int i = 1; i <= init_num; i++) {
		scanf("%d", &val);
		insert(ID[0], val, i);
		node_num++;	
	}

	char op;
	for (int i = init_num+1; i <= op_num + init_num; i++) {
		int c;
		while((c = getchar()) != '\n' && c != EOF);
		scanf("%c", &op);
		switch(op) {
			case 'H':
				scanf("%d", &val);
				node_t *new_node = insert(ID[0], val);
				move_head(new_node, head);
				node_num++;
				break;
			case 'T':
			       	scanf("%d", &val);
				insert(ID[0], val, i);
				node_num++;
				break;
			case 'A':
				scanf("%d %d", &id, &val);
				insert(ID[id]->next, val, i);
				node_num++;
				break;
			case 'B':
				scanf("%d %d", &id, &val);
				insert(ID[id], val, i);
				node_num++;
				break;
			case 'D':
				scanf("%d", &id);
				delete(ID[id], id);
				node_num--;
				break;
			case 'M':
				scanf("%c", &op);
				if(op == 'H') {
					scanf("%d", &id);
					move_head(ID[id], head);
				}
				else if (op == 'T') {
					scanf("%d", &id);
					move_tail(ID[id], tail);
				}
				break;
		}
	}

	if (node_num == 0) {
		printf("Empty\nEmpty");
		return 0;
	}
	node_t *curr;
	for (curr = head->next; curr->next != NULL; curr = curr->next) {
		printf("%d ", curr->val);
	}
	printf("\n");
	for (curr = tail->prev; curr->prev != NULL; curr = curr->prev) {
		printf("%d ", curr->val);
	}
	list_free(head);
	return 0;
}

