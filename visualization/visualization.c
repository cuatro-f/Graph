#include <stdio.h>
#include <string.h>
#include "visualization.h"
#include "../graph/graph.h"

void visualizate_graph(Graph* graph, FILE* file) {
	fprintf(file, "digraph BST {\n");
    	fprintf(file, "    node [fontname=\"Arial\"];\n");
	for (int i = 0; i < graph->adj_list->msize; i++) {
		if (graph->adj_list->ks[i]->busy == 1) {
			Node *current = graph->adj_list->ks[i]->adj_list->head;
			if (current == NULL) {
				fprintf(file, "    %s;\n", graph->adj_list->ks[i]->key);
			}
			while (current) {
				fprintf(file, "    %s->%s [label=%d];\n", graph->adj_list->ks[i]->key, current->vertex->key, current->relation);
				current = current->next;
			}
		}
	}
	fprintf(file, "}\n");
}

