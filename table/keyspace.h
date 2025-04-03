#ifndef KEYSPACE_H
#define KEYSPACE_H

#include <stdlib.h>
#include "../graph/graph.h"
#include "../list/list.h"

typedef struct List List;
typedef struct Vertex Vertex;

typedef struct KeySpace {
	int busy;
	char *key;
	Vertex *vertex;
	List *adj_list;
} KeySpace;

KeySpace* create_keyspace();
void print_keyspace(KeySpace*);
void free_keyspace(KeySpace*);
#endif
