#include <stdlib.h>
#include <stdio.h>
#include "graph.cpp"
#include "louvain.cpp"
#include <set>

int main(int argc, char **argv) {

	graph_t* graph = createGraphFromFile(argv[1]);

	graph->comm = (int*) malloc(graph->size * (sizeof(int)));

	for(int i  = 0; i < graph->size; ++i) {
		if(i == 0) {graph->comm[i] = 0;}
		if(i == 1 || i == 4 || i == 7 || i == 8) {graph->comm[i] = 1;}
		if(i == 2 || i == 3 || i == 5 || i == 6) {graph->comm[i] = 2;}
	}
	graph->nComm = 3; // Usually for regular graphs will be equal to graph->size
	buildCommunitySets(graph);
	computeSigmas(graph);

	// phase1(graph);
	// for(int i = 0; i < graph->size; ++i) {
	// 	printf("%d\n", graph->comm[i]);
	// }
	// for (int i = 0; i < graph->size; i++) {
	// 	vector<edge_t> neighbors = graph->nodes[i].neighbors;
	// 	printf("Degree of node[%d] = %d\n", i, graph->nodes[i].degree);
	// 	for(int j = 0; j < neighbors.size(); ++j) {
	// 		printf("%d -> %d\n", i, neighbors[j].target);
	// 	}
	// }
	return 1;
}