all: dialog.o main.o keyspace.o tree.o visualization.o list.o table.o queue.o
	gcc -o main -lreadline -lm dialog.o keyspace.o main.o input/read.o graph.o visualization.o list.o table.o queue.o
dialog.o: dialog/dialog.c
	gcc -c -o dialog.o dialog/dialog.c
main.o: main.c
	gcc -c -o main.o main.c
tree.o: graph/graph.c
	gcc -c -o graph.o graph/graph.c
visualization.o: visualization/visualization.c
	gcc -c -o visualization.o visualization/visualization.c
keyspace.o: table/keyspace.c
	gcc -c -o keyspace.o table/keyspace.c
table.o: keyspace.o table/table.c
	gcc -c -o table.o table/table.c
list.o:
	gcc -c -o list.o list/list.c
queue.o:
	gcc -c -o queue.o queue/queue.c
clean:
	rm -rf main
	rm -rf time
	rm -rf *.dot *.o
