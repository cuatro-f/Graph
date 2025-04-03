#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"
#include "../queue/queue.h"
#include "../list/list.h"
#include "../table/keyspace.h"
#include "../table/table.h"
#include "../input/read.h"

Graph* create_graph() {
	size_t msize = 500;
	Graph *graph = calloc(1, sizeof(Graph));
	graph->adj_list = create_table(msize);
	return graph;
}

void free_graph(Graph *graph) {
	free_table(graph->adj_list);
	free(graph);
}

void free_vertex(Vertex *vertex) {
	free(vertex);
}

void print_graph(Graph *graph) {
	if (graph == NULL) {
		return;
	}
	print_table(graph->adj_list);
}

Vertex* create_vertex(char *key) {
	Vertex *vertex = calloc(1, sizeof(Vertex));
	vertex->key = key;
	return vertex;
}

int add_vertex(Graph *graph, char *key) {
	int check = insert(graph->adj_list, key);
	return check;
}

int change_vertex_data(Graph *graph, char *key, char *new_key) {
	KeySpace *ks = find_by_key(graph->adj_list, key);
	Vertex *vertex = ks->vertex;
	List *adj_list = ks->adj_list;
	int check = change_by_key(graph->adj_list, key);
	check = insert(graph->adj_list, new_key);
	graph->adj_list->ks[check]->adj_list = adj_list;
	graph->adj_list->ks[check]->vertex = vertex;
	return 0;
}

int add_edge(Graph* graph, char *start, char *end, int relation) {
	if (strcmp(start, end) == 0) {
		return 1;
	}
	KeySpace *first = find_by_key(graph->adj_list, start);
	if (first == NULL) {
		return 1;
	}
	KeySpace *second = find_by_key(graph->adj_list, end);
	if (second == NULL) {
		return 1;
	}
	Node *current = first->adj_list->head;
	while (current) {
		if (strcmp(current->vertex->key, end) == 0) {
			return 1;
		}
		current = current->next;
	}
	int check = push(first->adj_list, second->vertex, relation);
	return 0;

}

int change_edge_data(Graph *graph, char *start, char *end, int relation) {
	if (strcmp(start, end) == 0) {
		return 1;
	}
	KeySpace *ks = find_by_key(graph->adj_list, start);
	if (ks == NULL) {
		return 1;
	}
	Node *current = ks->adj_list->head;
	while (current) {
		if (strcmp(current->vertex->key, end) == 0) {
			current->relation = relation;
			return 0;
		}
		current = current->next;
	}
	return 1;
}

int remove_vertex(Graph *graph, char *name) {
	for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy == 1) {
			int check = remove_node(graph->adj_list->ks[i]->adj_list, name);
		}
	}
	int check = remove_by_key(graph->adj_list, name);
	return check;
}

int remove_edge(Graph *graph, char *start, char *end) {
	KeySpace *ks = find_by_key(graph->adj_list, start);
	if (ks == NULL) {
		return -1;
	}
	int check = remove_node(ks->adj_list, end);
	return check;
}

void init(Graph *graph) {
	for (int i = 0; i < graph->adj_list->msize; i++) {
                if (graph->adj_list->ks[i]->busy != 1) {
                        continue;
                }
                if (graph->adj_list->ks[i]->adj_list == NULL) {
                        continue;
                }
		graph->adj_list->ks[i]->vertex->distance = INT_MAX;
		graph->adj_list->ks[i]->vertex->previous = NULL;
                Node *current = graph->adj_list->ks[i]->adj_list->head;
                while (current) {
                        current->vertex->color = white;
                        current->vertex->previous = NULL;
                        current->vertex->distance = INT_MAX;
                        current = current->next;
                }
        }	
} 

List* width_first_search(Graph *graph, char *start, int depth) {
	init(graph);
	KeySpace *ks = find_by_key(graph->adj_list, start);
	if (ks == NULL) {
		return NULL;
	}
	ks->vertex->color = gray;
	ks->vertex->distance = 0;
	Queue *queue = create_queue();
	put(queue, ks->vertex);
	List *familiar = create_list();
	while (!isEmpty(queue)) {
		Vertex *vertex = pop(queue);
		if (vertex == NULL) {
			free_queue(queue);
			return familiar;
		}
		KeySpace *element = find_by_key(graph->adj_list, vertex->key);
		if (element == NULL) {
			free_queue(queue);
			return familiar;
		}
		if (element->adj_list == NULL) {
			printf("continue\n");
			continue;
		}
		Node *node = element->adj_list->head;
		while (node) {
			if (node->vertex->color == white) {
				node->vertex->color = gray;
				node->vertex->distance = vertex->distance + 1;
				node->vertex->previous = vertex;
				if (node->vertex->distance <= depth) {
					push(familiar, node->vertex, 0);
				}
				put(queue, node->vertex);
			}
			node = node->next;
		}
		vertex->color = black;
	}
	free_queue(queue);
	return familiar;
}

/*List* shortest_way(Graph *graph, char *start, char *end) {
	if (strcmp(start, end) == 0) {
		return NULL;
	}
	init(graph);
	KeySpace *ks = find_by_key(graph->adj_list, start);
	if (ks == NULL) {
		return NULL;
	}
	KeySpace *found = find_by_key(graph->adj_list, end);
	if (found == NULL) {
		return NULL;
	}
	ks->vertex->distance = 0;
	for (int i = 1; i < graph->adj_list->csize - 1; i++) {
		for (int j = 0; j < graph->adj_list->msize; j ++) {
			if (graph->adj_list->ks[j]->busy != 1) {
				continue;
			}
			Node *node = graph->adj_list->ks[j]->adj_list->head;
			while (node) {
				if (graph->adj_list->ks[j]->vertex->distance != INT_MAX && node->vertex->distance > graph->adj_list->ks[j]->vertex->distance + node->relation) {
					node->vertex->distance = graph->adj_list->ks[j]->vertex->distance + node->relation;
					node->vertex->previous = graph->adj_list->ks[j]->vertex;
				}
				node = node->next;
			}
		}
	}
	for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy != 1) {
			continue;
		}
		Node *node = graph->adj_list->ks[i]->adj_list->head;
		while (node) {
			if (node->vertex->distance > graph->adj_list->ks[i]->vertex->distance + node->relation && graph->adj_list->ks[i]->vertex->distance != INT_MAX) {
				printf("<0 cycle\n");
				return NULL;
			}
			node = node->next;
		}
	}
	if (found->vertex->distance == INT_MAX) {
		printf("here\n");
		// the node is unreachable
		return NULL;
	}
	Vertex *vertex = found->vertex;
	List *list = create_list();
	while (vertex) {
		printf("ge\n");
		push(list, vertex, 0);
		vertex = vertex->previous;
	}
	return list;
}*/

List* shortest_way(Graph *graph, char *from, char *to) {
	if (strcmp(from, to) == 0) {
		return NULL;
	}
	KeySpace *start = find_by_key(graph->adj_list, from);
	if (start == NULL) {
		return NULL;
	}
	KeySpace *end = find_by_key(graph->adj_list, to);
	if (end == NULL) {
		return NULL;
	}
	for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy == 1) {
			graph->adj_list->ks[i]->vertex->distance = INT_MAX;
			graph->adj_list->ks[i]->vertex->previous = NULL;
		}
	}
	start->vertex->distance = 0;
	for (int j = 1; j < graph->adj_list->csize - 1; j++) {
		for (int i = 0; i < graph->adj_list->msize; i++) {
			if (graph->adj_list->ks[i] == NULL) {
				continue;
			}
			if (graph->adj_list->ks[i]->busy != 1) {
				continue;
			}
			if (graph->adj_list->ks[i]->adj_list == NULL) {
				continue;
			}
			Node *node = graph->adj_list->ks[i]->adj_list->head;
			while (node) {
				if (graph->adj_list->ks[i]->vertex == NULL) {
					node = node->next;
					continue;
				}
				if (graph->adj_list->ks[i]->vertex->distance != INT_MAX && node->vertex->distance > graph->adj_list->ks[i]->vertex->distance + node->relation) {
					node->vertex->distance = graph->adj_list->ks[i]->vertex->distance + node->relation;
					node->vertex->previous = graph->adj_list->ks[i]->vertex;
				}
				node = node->next;
			}
		}
	}
	/*for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy != 1) {
			continue;
		}
		Node *node = graph->adj_list->ks[i]->adj_list->head;
		while (node) {
			if (graph->adj_list->ks[i]->vertex->distance != INT_MAX && node->vertex->distance > graph->adj_list->ks[i]->vertex->distance + node->relation) {
				printf("No way again\n");
				return NULL;
			}
			node = node->next;
		}
	}*/
	if (end->vertex == NULL) {
		return NULL;
	}
	if (end->vertex->distance == INT_MAX) {
		printf("unreachable\n`");
		return 0;
	}
	List *list = create_list();
	Vertex *node = end->vertex;
	while (node) {
		push(list, node, 0);
		node = node->previous;
		if (node == NULL) {
			return list;
		}
		Node *top = list->head;
		while (top) {
			if (strcmp(node->key, top->vertex->key) == 0) {
				printf("infinite loop\n");
				push(list, node, 0);
				return list;
			}
			top = top->next;
		}
	}
	return list;
}

int loop(int size, int distance[size][size]) {
	for (int i = 0; i < size; i++) {
		if (distance[i][i] > 0) {
			return 1;
		}
	}
	return 0;
}

Vertex* longest_way(Graph *graph, char *from) {
	int distance[graph->adj_list->msize][graph->adj_list->msize];
	for (int i = 0; i < graph->adj_list->msize; i++) {
		for (int j = 0; j < graph->adj_list->msize; j++) {
			distance[i][j] = INT_MIN;
		}
	}
	for (int i = 0; i < graph->adj_list->msize; i ++) {
		if (graph->adj_list->ks[i]->busy != 1) {
			for (int index = 0; index < graph->adj_list->msize; index++) {
				distance[i][index] = INT_MIN;
			}
			continue;
		}
		for (int j = 0; j < graph->adj_list->msize; j++) {
			if (graph->adj_list->ks[j]->busy != 1) {
				distance[i][j] = INT_MIN;
				continue;
			}
			Node *node = graph->adj_list->ks[i]->adj_list->head;
			while (node) {
				if (strcmp(node->vertex->key, graph->adj_list->ks[j]->key) == 0) {
					distance[i][j] = node->relation;
				}
				node = node->next;
			}
		}
	}
	/*for (int i = 0; i < graph->adj_list->msize; i++) {
                for (int j = 0; j < graph->adj_list->msize; j++) {
                        if (distance[i][j] == INT_MIN) {
                                printf("%s ", "INF");
                        }
                        else {
                                printf("%d ", distance[i][j]);
                        }
                }
                printf("\n");
        }*/
	for (int k = 0; k < graph->adj_list->msize; k++) {
		for (int i = 0; i < graph->adj_list->msize; i++) {
			for (int j = 0; j < graph->adj_list->msize; j++) {
				if (distance[i][k] != INT_MIN && distance[k][j] != INT_MIN && distance[i][k] + distance[k][j] > distance[i][j]) {
					distance[i][j] = distance[i][k] +  distance[k][j];
				}
			}
		}
	}
	/*for (int i = 0; i < graph->adj_list->msize; i++) {
		for (int j = 0; j < graph->adj_list->msize; j++) {
			if (distance[i][j] == INT_MIN) {
				printf("%s ", "INF");
			}
			else {
				printf("%d ", distance[i][j]);
			}
		}
		printf("\n");
	}*/
	if (loop(graph->adj_list->msize, distance)) {
		printf("There is loop in graph\n");
		return NULL;
	}
	Vertex *found = NULL;
	for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy != 1) {
			continue;
		}
		if (strcmp(graph->adj_list->ks[i]->key, from) == 0) {
			int max = INT_MIN;
			for (int j = 0; j < graph->adj_list->msize; j++) {
				if (distance[i][j] > max) {
					max = distance[i][j];
					found = graph->adj_list->ks[j]->vertex;
				}
			}
		}
	}
	return found;
}

void import_file(Graph *graph, char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return;
	}
	char *names = myreadfile(file);
	char *name = strtok(names, " ");
	while (name) {
		char *in = strdup(name);
		int check = add_vertex(graph, in);
		name = strtok(NULL, " ");
	}
	free(names);
	char *string = NULL;
	do {
		string = myreadfile(file);
		if (string == NULL) {
			fclose(file);
			return;
		}
		char *start = strtok(string, " ");
		if (start == NULL) {
			free(string);
			break;
		}
		char *start_in = strdup(start);
		char *end = strtok(NULL, " ");
		if (end == NULL) {
			free(string);
			break;
		}
		char *end_in = strdup(end);
		char *relation = strtok(NULL, " ");
		if (relation == NULL) {
			free(string);
			break;
		}
		int relation_in = atoi(relation);
		int check = add_edge(graph, start_in, end_in, relation_in);
		free(string);
		free(start_in);
		free(end_in);
	} while (string != NULL);
	fclose(file);
	return;
}

void export_graph(Graph *graph, char *filename) {
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		return;
	}
	for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy != 1) {
			continue;
		}
		char *key = graph->adj_list->ks[i]->key;
		int check = fwrite(key, strlen(key), sizeof(char), file);
		if (i != graph->adj_list->msize - 1) {
			check = fwrite(" ", 1, sizeof(char), file);
		}
	}
	int check = fwrite("\n", 1, sizeof(char), file);
	for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy != 1) {
			continue;
		}
		Node *node = graph->adj_list->ks[i]->adj_list->head;
		while (node) {
			fprintf(file, "%s %s %d\n", graph->adj_list->ks[i]->key, node->vertex->key, node->relation);
			node = node->next;
		}
	}
	fclose(file);
}

