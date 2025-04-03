#ifndef QUEUE_H
#define QUEUE_G
#include "../graph/graph.h"

typedef struct Unit {
	Vertex *node;
	struct Unit *next;
} Unit;

typedef struct Queue {
	Unit *head;
	Unit *tail;
} Queue;

Queue *create_queue();
Unit *create_unit(Vertex*);
void free_unit(Unit*);
void free_queue(Queue*);
int put(Queue*, Vertex*);
Vertex *pop(Queue*);
int isEmpty(Queue*);

#endif
