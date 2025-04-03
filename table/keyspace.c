#include <stdlib.h>
#include <stdio.h>
#include "keyspace.h"
#include "table.h"
#include "../list/list.h"

// создается пустой keyspace
KeySpace* create_keyspace() {
	KeySpace* keyspace = calloc(1, sizeof(KeySpace));
	if (keyspace == NULL) {
		return NULL;
	}
	keyspace->busy = 0;
	keyspace->key = NULL;
	keyspace->vertex = NULL;
	keyspace->adj_list = NULL;
	return keyspace;
}

void print_keyspace(KeySpace* keyspace) {
	if (keyspace->busy != 1) {
		return;
	}
	printf("key %s: ", keyspace->key);
	print_list(keyspace->adj_list);
	printf("\n");
}

void free_keyspace(KeySpace *keyspace) {
	if (keyspace->busy == 1) {
		free(keyspace->key);
		free_list(keyspace->adj_list);
		free_vertex(keyspace->vertex);
	}
	free(keyspace);
}
