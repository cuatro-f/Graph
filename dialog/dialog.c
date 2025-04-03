#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dialog.h"
#include "../graph/graph.h"
#include <readline/readline.h>
#include "../visualization/visualization.h"
#include "../input/read.h"

int get_key(char **key, char *prompt) {
	*key = readline(prompt);
    	if (*key == NULL) {
		return EOF;
 	}
    	return 0;
}

int get_value(int value) {
	return 1;
}

int bigger_than_zero(int value) {
	return value > 0;
}

int get_index(int index) {
    return (index >= 0 && index <= 13);
}

int check_relation(int relation) {
	return (relation <= 10 && relation >= -10);
}

int get_int(int* number, int(*func)(int), char* str) {
    int flag = 1;
    do {
        printf("%s: ", str);
        int check = scanf("%zu", number);
        if (check == EOF) {
            return check;
        }
        if (func(*number) == 1 && check == 1) {
            return 0;
        }
	printf("\n");
        scanf("%*[^\n]");
        scanf("%*c");
    } while (flag);
    return 1;
}

int dialog(const char* msgs[], const int len) {
    char* errmsg = "";
    int check = 0;
    int number = 0;
    do {
        puts(errmsg);
        errmsg = "Repeate input";
        for (int i = 0; i < len; i++) {
            puts(msgs[i]);
        }
        check = get_int(&number, &get_index, "choose one option");
        if (check == EOF) {
            return 0;
        }
    } while (number < 0 || number >= len);
    return number;
}

int process(Graph* graph) {
    const char* msgs[] = { "0. Quit", "1. Add vertex", "2. Add edge", "3. Remove vertex", "4. Remove edge", "5. Change vertext data", 
    "6. Change edge data", "7. Print", "8. Visualization", "9. Graph traversal", "10. Find shortest way", "11. Special operation", "12. Import graph", "13. Export graph"};
    const int len = sizeof(msgs) / sizeof(msgs[0]);
    int check = 0;
    int flag = 1;
    int (*funcs[]) (Graph*) = {&dialog_exit, &dialog_add_vertex, &dialog_add_edge, &dialog_remove_vertex, &dialog_remove_edge, &dialog_change_vertex_data, &dialog_change_edge_data, &dialog_print, &dialog_visualization, &dialog_traversal, &dialog_find_shortest_way, &dialog_special_operation, &dialog_import, &dialog_export}; 
    while (flag) {
        int index = dialog(msgs, len);
	check = funcs[index](graph);
	if (check == -1) {
		flag = 0;
		return EOF;
	}
    }
}

int dialog_exit(Graph* graph) {
	return -1;
}

int dialog_add_vertex(Graph *graph) {
	char *key = NULL;
	int check = get_key(&key, "Input name: ");
	if (check == EOF) {
		return EOF;
	}
	check = add_vertex(graph, key);
	if (check == -1) {
		free(key);
	}
	return 0;
}

int dialog_add_edge(Graph *graph) {
	char *start = NULL, *end = NULL;
	int relation = 0;
	int check = get_key(&start, "Input start point: ");
	if (check == EOF) {
		return EOF;
	}
	check = get_key(&end, "Input end point: ");
	if (check == EOF) {
		free(start);
		return EOF;
	}
	check = get_int(&relation, &check_relation, "Input relation: ");
	if (check == EOF) {
		free(start);
		free(end);
		return EOF;
	}
	check = add_edge(graph, start, end, relation);
	free(end);
	free(start);
	return 0;
}

int dialog_remove_vertex(Graph *graph) {
	char *key = NULL;
	int check = get_key(&key, "Input vertex for romove: ");
	if (check == EOF) {
		return EOF;
	}
	check = remove_vertex(graph, key);
	free(key);
	return check;
}

int dialog_remove_edge(Graph *graph) {
	char *start = NULL, *end = NULL;
	int check = get_key(&start, "Input start point: ");
	if (check == EOF) {
		return EOF;
	}
	check = get_key(&end, "Input end point: ");
	if (check == EOF) {
		free(start);
		return EOF;
	}
	check = remove_edge(graph, start, end);
	free(start);
	free(end);
	return check;
}

int dialog_change_vertex_data(Graph *graph) {
	char *key = NULL, *new_key = NULL;
	int check = get_key(&key, "Input name for changing: ");
	if (check == EOF) {
		return EOF;
	}
	check = get_key(&new_key, "Input new name: ");
	if (check == EOF) {
		free(key);
		return EOF;
	}
	check = change_vertex_data(graph, key, new_key);
	return 0;
}

int dialog_change_edge_data(Graph *graph) {
	int relation = 0;
	char *start = NULL, *end = NULL;
        int check = get_key(&start, "Input start point: ");
        if (check == EOF) {
                return EOF;
        }
        check = get_key(&end, "Input end point: ");
        if (check == EOF) {
                free(start);
                return EOF;
        }
        check = get_int(&relation, &check_relation, "Input relation: ");
        if (check == EOF) {
                free(start);
                free(end);
                return EOF;
        }
        check = change_edge_data(graph, start, end, relation);
        free(end);
        free(start);
        return 0;
}
 
int dialog_print(Graph *graph) {
	print_graph(graph);
	return 0;
}

int dialog_visualization(Graph *graph) {
	scanf("%*c");
	char *filename = readline("Input filename ");
	if (filename == NULL) {
		printf("filename error\n");
		return 0;
	}
	remove_endspace(filename);
	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		free(filename);
		return 1;
	}
	visualizate_graph(graph, file);
	fclose(file);
	free(filename);
	return 0;
}

int dialog_traversal(Graph *graph) {
	char *name = NULL;
	int depth = 0;
	int check = get_key(&name, "Input name: ");
	if (check == EOF) {
		return EOF;
	}
	check = get_int(&depth, &bigger_than_zero, "Input number of handshakes");
	if (check == EOF) {
		free(name);
		return EOF;
	}
	List *familiar = width_first_search(graph, name, depth);
	print_list(familiar);
	free_list(familiar);
	free(name);
	return 0;
}

int dialog_find_shortest_way(Graph *graph) {
	char *start = NULL, *end = NULL;
	int check = get_key(&start, "Input start node: ");
	if (check == EOF) {
		return EOF;
	}
	check = get_key(&end, "Input end node: ");
	if (check == EOF) {
		free(start);
		return EOF;
	}
	List *list = shortest_way(graph, start, end);
	print_list(list);
	free_list(list);
	free(start);
	free(end);
	return 0;
}

int dialog_special_operation(Graph *graph) {
	char *from = NULL;
	int check = get_key(&from, "Input start node: ");
	if (check == EOF) {
		return EOF;
	}
	Vertex *vertex = longest_way(graph, from);
	free(from);
	if (vertex == NULL) {
		printf("No longest way for this vertex\n");
		return 1;
	}
	printf("longes way is way to vertex %s\n", vertex->key);
	return 0;
}

int dialog_import(Graph *graph) {
	char *filename = readline("Input filename ");
	if (filename == NULL) {
		printf("filename error\n");
		return 0;
	}
	remove_endspace(filename);
	import_file(graph, filename);
	free(filename);
	return 0;
}

int dialog_export(Graph *graph) {
	char *filename = readline("Input filename ");
	if (filename == NULL) {
		printf("filename error\n");
		return 0;
	}
	remove_endspace(filename);
	export_graph(graph, filename);
	free(filename);
	return 0;
}
