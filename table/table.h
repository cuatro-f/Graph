#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include "keyspace.h"
typedef struct KeySpace KeySpace;

typedef struct Table {
	KeySpace **ks;
	size_t msize;
	size_t csize;
} Table;

int first_hash(char*);
int second_hash(size_t, char*);
int common_hash(size_t, char*, int);
Table* create_table(size_t);
int insert(Table*, char*);
int remove_by_key(Table*, char*);
KeySpace *find_by_key(Table*, char*);
int change_by_key(Table*, char*);
void print_table(Table*);
void free_table(Table*);
int find(Table*, char*);
int expansion(Table *table);
#endif
