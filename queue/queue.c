#include "queue.h"
#include "../graph/graph.h"

Queue *create_queue() {
	Queue *queue = calloc(1, sizeof(Queue));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

Unit *create_unit(Vertex *vertex) {
	Unit *unit = calloc(1, sizeof(Unit));
	unit->node = vertex;
	unit->next = NULL;
	return unit;
}

void free_unit(Unit *unit) {
	free(unit);
}

void free_queue(Queue *queue) {
	Unit *top = queue->head, *prev = NULL;
	while (top) {
		prev = top;
		top = top->next;
		free_unit(prev);
	}
	free(queue);
}

int put(Queue *queue, Vertex *vertex) {
	if (queue->head == NULL) {
		Unit *new = create_unit(vertex);
		queue->head = new;
		queue->tail = new;
		return 0;
	}
	queue->tail->next = create_unit(vertex);
	queue->tail = queue->tail->next;
	return 0;
}

Vertex* pop(Queue *queue) {
	if (queue->head == NULL) {
		return NULL;
	}
	Unit *tmp = queue->head;
	Vertex *temp = tmp->node;
	queue->head = queue->head->next;
	free(tmp);
	return temp;
}

int isEmpty(Queue *queue) {
	return queue->head == NULL;
}
