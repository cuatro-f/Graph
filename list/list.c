#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "../graph/graph.h"

List* create_list() {
	List *list = calloc(1, sizeof(List));
	list->head = NULL;
}

Node* create_node(Vertex *vertex, Node *next, int relation) {
	Node *node = calloc(1, sizeof(Node));
	node->vertex = vertex;
	node->relation = relation;
	node->next = next;
}

int push(List *list, Vertex *vertex, int relation) {
	Node *new = create_node(vertex, list->head, relation);
	list->head = new;
	return 0;
}

int remove_node(List *list, char *key) {
	Node *current = list->head;
	Node *prev = NULL;
	while (current) {
		if (strcmp(current->vertex->key, key) == 0) {
			if (current == list->head) {
				list->head = current->next;
				free(current);
				return 0;
			}
			prev->next = current->next;
			free(current);
			return 0;
		}
		prev = current;
		current = current->next;
	}
	return 0;
}

void free_list(List *list) {
	if (list == NULL) {
		return;
	}
	Node *current = list->head, *prev = NULL;
	while (current) {
		prev = current;
		current = current->next;
		free(prev);
	}
	free(list);
}

void print_list(List *list) {
	if (list == NULL) {
		return;
	}
	Node *current = list->head;
	while (current) {
		printf("%s key: %d relation distance: %d -> ", current->vertex->key, current->relation, current->vertex->distance);
		current = current->next;
	}
}

