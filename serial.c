#include "graph.h"
int main(int argc, char **argv) {

	hashtable_t * table = (hashtable_t *) malloc(sizeof(hashtable_t));
	createGraphFromFile(argv[1], table);
	for (int i = 0; i < table->size; i++) {
		neighbor_t* cur = table->buckets[i].head;
		printf("Degree of node[%d] = %d\n", i, table->buckets[i].neighborsCount);
		while (cur != NULL) {
			printf("%d -> %d\n", i, cur->node);
			cur = cur->next;
		}
	} 
	return 1;
}