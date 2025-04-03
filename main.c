#include <stdio.h>
#include "graph/graph.h"
#include "dialog/dialog.h"
#include "table/table.h"
#include "graph/graph.h"
#include "table/keyspace.h"

int main() {
	Graph *graph = create_graph();
	if (graph == NULL) {
		return 1;
	}
	int check = process(graph);
	free_graph(graph);
	return 0;
}
