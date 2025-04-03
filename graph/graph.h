#ifndef GRAPH_H
#define GRAPH_H

#include "../table/table.h"

enum COLORS {
	white = 0,
	gray,
	black,
};

typedef struct Table Table;
typedef struct List List;

typedef struct Vertex {
	char *key;
	char color;
	int distance;
	struct Vertex *previous;
} Vertex;

typedef struct Graph {
	Table *adj_list;
} Graph;

Graph *create_graph();
Vertex *create_vertex(char*);
void free_graph(Graph*);
void free_vertex(Vertex*);
void print_graph(Graph*);
int add_vertex(Graph*, char*);
int change_vertex_data(Graph*, char*, char*);
int add_edge(Graph*, char*, char*, int);
int change_edge_data(Graph*, char*, char*, int);
int remove_vertex(Graph*, char*);
int remove_edge(Graph*, char*, char*);
void init(Graph*);
List* width_first_search(Graph*, char*, int);
List* shortest_way(Graph*, char*, char*);
Vertex* longest_way(Graph*, char*);
void import_file(Graph*, char*);
void export_graph(Graph*, char*);
#endif
