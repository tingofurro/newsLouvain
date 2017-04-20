#include <stdlib.h>
#include <stdio.h>
#include "graph.cpp"
#include "louvain.cpp"
#include <set>

int main(int argc, char **argv) {

	graph_t* graph = createGraphFromFile(argv[1]);
	graph_t* original_graph = graph;
	int* keepTrackComm = new int[graph->size];
	for(int i = 0; i < graph->size; ++i) {keepTrackComm[i] = i;}

	int numChanges = 1;
	while(numChanges > 0) {
		printf("Phase. Nodes: %d \n", graph->size);
		numChanges = phase1(graph);
		compactComm(graph);

		for(int i  = 0; i < original_graph->size; ++i) {
			keepTrackComm[i] = graph->comm[keepTrackComm[i]];
		}
		printf("Before merge\n");
		graph_t* newGraph = mergeGraph(graph);
		graph = newGraph;
		printf("Got here merge\n");
	}

	// for(int i = 0; i < original_graph->size; ++i) {
	// 	printf("%d\n", keepTrackComm[i]);
	// }
	return 1;
}