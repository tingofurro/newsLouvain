#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	hashtable_t * graph = (hashtable_t *) malloc(sizeof(hashtable_t));
	createGraphFromFile(argv[1], graph);

	int[] comm = {0,0,0,0,0,1,1,1,1};
	for(int i = 0; i < graph->size; i++) {
		printf("%d\n", comm[i]);
	}
	printf("WTF\n");
	for (int i = 0; i < graph->size; i++) {
		neighbor_t* cur = graph->buckets[i].head;
		printf("Degree of node[%d] = %d\n", i, graph->buckets[i].neighborsCount);
		while (cur != NULL) {
			printf("%d -> %d\n", i, cur->node);
			cur = cur->next;
		}
	}
	return 1;
}