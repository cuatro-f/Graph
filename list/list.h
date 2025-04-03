#ifndef LIST_H
#define LIST_H

#include "../graph/graph.h"

typedef struct Vertex Vertex;

typedef struct Node {
	Vertex *vertex;
	int relation;
	struct Node *next;
} Node;

typedef struct List {
	Node *head;
} List;

Node *create_node(Vertex*, Node*, int);
List *create_list();
int push(List*, Vertex*, int);
int remove_node(List*, char*);
void free_list(List*);
void print_list(List*);
#endif
