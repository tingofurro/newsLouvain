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
		numChanges = phase1(graph);
		compactComm(graph);

		for(int i  = 0; i < original_graph->size; ++i) {
			keepTrackComm[i] = graph->comm[keepTrackComm[i]];
		}
		graph_t* newGraph = mergeGraph(graph);
		graph = newGraph;
	}


	// for(int i  = 0; i < graph->size; ++i) {
	// 	if(i == 0) {graph->comm[i] = 0;}
	// 	if(i == 1 || i == 4 || i == 7 || i == 8) {graph->comm[i] = 1;}
	// 	if(i == 2 || i == 3 || i == 5 || i == 6) {graph->comm[i] = 2;}
	// }
	// computeSigmas(graph);
	// print_sigmas(graph);

	// graph_t* newGraph = mergeGraph(graph);
	// print_sigmas(newGraph);
	// printf("Removed node 0\n");
	// removeNode(newGraph, 0);

	// print_sigmas(newGraph);
	// printf("Add node 0 to 2\n");
	// addNode(newGraph, 0, 2);

	// print_sigmas(newGraph);

	for(int i = 0; i < original_graph->size; ++i) {
		printf("%d\n", keepTrackComm[i]);
	}
	return 1;
}