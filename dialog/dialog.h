#ifndef DIALOG_H
#define DIALOG_H
#include <stdlib.h>
#include "../graph/graph.h"

int dialog_exit(Graph*); //+  
int dialog_add_vertex(Graph*); //+
int dialog_add_edge(Graph*);
int dialog_remove_vertex(Graph*);
int dialog_remove_edge(Graph*);
int dialog_change_vertex_data(Graph*);
int dialog_change_edge_data(Graph*);
int dialog_print(Graph*);
int dialog_visualization(Graph*);
int dialog_traversal(Graph*);
int dialog_find_shortest_way(Graph*);
int dialog_special_operation(Graph*);
int dialog_import(Graph*);
int dialog_export(Graph*);
int process(Graph*);
int get_key(char**, char*);
int get_value(int);
int get_int(int*, int(*)(int), char*);
int dialog(const char**, const int);
#endif 
