#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "table.h"
#include "keyspace.h"

int first_hash(char *key) {
	int hash = 0;
	char *str = key;
	while (*str) {
        	hash ^= *str;
        	hash = (hash << 3) ^ (hash >> 13) ^ (hash >> 5);
        	str++;
    }
	return hash;
}

// hash 2 - не равен нулю
// hash 2 - прост по отношению к table_size
/*int second_hash(size_t msize, char *key) {
    	int hash = 0;
    	int i = 0;
	char *str = key;
    	while (*str) {
        	hash = (hash << 7) ^ (hash >> 25) ^ (hash >> 23);
        	hash = hash ^ ((*str) << 3);
        	str++;
    }
	if (hash == 0) {
		hash++;
	}
	while (hash % msize == 0) {
		hash++;
	}
    	return hash;
}*/

int second_hash(size_t msize, char *key) {
	return 1;
}

int common_hash(size_t msize, char* key, int iteration) {
	int first = first_hash(key), second = second_hash(msize, key);
	return first + second * iteration;
	
}

int find(Table *table, char* key) {
	int index = first_hash(key) % table->msize;
	int check = 0, new_position = index, iteration = 1;
	do {
		if (table->ks[new_position]->busy == 0) {
			return 0;
		}
		if (table->ks[new_position]->busy == -1) {
			new_position = common_hash(table->msize, key, iteration) % table->msize;
			iteration++;
			continue;
		}
		if (strcmp(table->ks[new_position]->key, key) == 0) {
			return 1;
		}
		new_position = common_hash(table->msize, key, iteration) % table->msize;
		iteration++;
	} while (index != new_position);
	return 0;
}

int insert(Table *table, char *key) {
	if (table->msize == table->csize && find(table, key) == 0) {
		int check = expansion(table);
	}
	int index = first_hash(key) % table->msize;
	int check = 0, new_position = index, iteration = 1;
	do {
		if (table->ks[new_position]->busy == 1 && strcmp(table->ks[new_position]->key, key) == 0) {
			return -1;
		}
		if (table->ks[new_position]->busy == 0 || table->ks[new_position]->busy == -1) {
			table->ks[new_position]->busy = 1;
			table->ks[new_position]->key = key;
			table->ks[new_position]->adj_list = create_list();
			table->ks[new_position]->vertex = create_vertex(key);
			(table->csize)++;
			return new_position;
		}
		new_position = common_hash(table->msize, key, iteration) % table->msize;
		iteration++;
	} while (new_position != index);
	free(key);
	// не получилось вставить
	return -1;
}

Table* create_table(size_t msize) {
	Table *table = calloc(1, sizeof(Table));
	if (table == NULL) {
		return NULL;
	}
	table->ks = calloc(msize, sizeof(KeySpace*));
	if (table->ks == NULL) {
		free(table);
		return NULL;
	}
	for (int i = 0; i < msize; i++) {
		table->ks[i] = create_keyspace();
	}
	table->csize = 0;
	table->msize = msize;
	printf("successful create ");
	printf("%d %d\n", table->msize, table->csize);
	return table;
}

void print_table(Table* table) {
	for (int i = 0; i < table->msize; i++) {
		if (table->ks[i]->busy == 1) {
			print_keyspace(table->ks[i]);
		}
	}
}

void free_table(Table *table) {
	for (int i = 0; i < table->msize; i++) {
		free_keyspace(table->ks[i]);
	}
	free(table->ks);
	free(table);
}

int remove_by_key(Table *table, char *key) {
	int index = first_hash(key) % table->msize;
	int new_position = index, iteration = 1;
	do {
		if (table->ks[new_position]->busy == 0) {
			return 1;
		}
		else if (table->ks[new_position]->busy == -1) {
			new_position = common_hash(table->msize, key, iteration) % table->msize;
			iteration++;
			continue;
		}
		if (strcmp(table->ks[new_position]->key, key) == 0) {
			// busy = 1 and ks->key = key
			table->ks[new_position]->busy = -1;
			free(table->ks[new_position]->key);
			table->ks[new_position]->key = NULL;
			free_vertex(table->ks[new_position]->vertex);
			table->ks[new_position]->vertex = NULL;
			free_list(table->ks[new_position]->adj_list);
			table->ks[new_position]->adj_list = NULL;
			(table->csize)--;
			return 0;
		}
	} while (new_position != index);
	// не удалось убрать элемент
	return 1;
}

int change_by_key(Table *table, char *key) {
	int index = first_hash(key) % table->msize;
	int new_position = index, iteration = 1;
	do {
		if (table->ks[new_position]->busy == 0) {
			return 1;
		}
		else if (table->ks[new_position]->busy == -1) {
			new_position = common_hash(table->msize, key, iteration) % table->msize;
			iteration++;
			continue;
		}
		if (strcmp(table->ks[new_position]->key, key) == 0) {
			free(table->ks[new_position]->key);
			table->ks[new_position]->key = NULL;
			table->ks[new_position]->vertex = NULL;
			table->ks[new_position]->adj_list = NULL;
			table->ks[new_position]->busy = -1;
			(table->csize)--;
			return 0;
		}
	} while (new_position != index);
}

KeySpace* find_by_key(Table *table, char *key) {
	int index = first_hash(key) % table->msize;
	int new_position = index, iteration = 1;
	do {
		if (table->ks[new_position]->busy == 0) {
			return NULL;
		}
		else if (table->ks[new_position]->busy == -1) {
			new_position = common_hash(table->msize, key, iteration) % table->msize;
			iteration++;
			continue;
		}
		if (strcmp(table->ks[new_position]->key, key) == 0) {
			return table->ks[new_position];
		}
		new_position = common_hash(table->msize, key, iteration) % table->msize;
		iteration++;
	} while (new_position != index);
	return NULL;
}

int is_prime(size_t number) {
	if (number <= 1) {
		return 0;
	}
	for (size_t i = 2; i < sqrt(number) + 1; i++) {
		if (number % i == 0) {
			return 0;
		}
	}
	return 1;
}

int expansion(Table *table) {
	size_t new = (table->msize) + 1;
	while (is_prime(new) != 1) {
		new++;
	}
	Table *new_table = create_table(new);
	for (int i = 0; i < table->msize; i++) {
		if (table->ks[i]->busy == 1) {
			int check = insert(new_table, table->ks[i]->key);
			if (check == -1) {
				continue;
			}
			free_vertex(new_table->ks[check]->vertex);
			free_list(new_table->ks[check]->adj_list);
			new_table->ks[check]->vertex = table->ks[i]->vertex;
			new_table->ks[check]->adj_list = table->ks[i]->adj_list;
		}
		free(table->ks[i]);
	}
	free(table->ks);
	table->msize = new_table->msize;
	table->csize = new_table->csize;
	table->ks = new_table->ks;
	free(new_table);
	return 0;
}
