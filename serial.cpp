#include <stdlib.h>
#include <stdio.h>
#include "graph.cpp"
#include "louvain.cpp"
#include <set>

int main(int argc, char **argv) {

	graph_t* graph = createGraphFromFile(argv[1]);

	int comm[] = {0,6,1,1,2,2,3,3, 4, 5, 6, 7, 8};

	for(int i  = 0; i < graph->size; ++i) {
		if(i == 0) {comm[i] = 0;}
		if(i == 1 || i == 4 || i == 7 || i == 8) {comm[i] = 1;}
		if(i == 2 || i == 3 || i == 5 || i == 6) {comm[i] = 2;}
	}

	std::set<int> comm_set = neighborCommunities(&graph->nodes[0], comm);
	graph_t* newGraph = mergeGraph(graph, comm);

	// while(!comm_set.empty()) {
	// 	printf("%d\n", *comm_set.begin());
	// 	comm_set.erase(comm_set.begin());
	// }

	for (int i = 0; i < newGraph->size; i++) {
		vector<edge_t> neighbors = newGraph->nodes[i].neighbors;
		printf("Degree of node[%d] = %d\n", i, newGraph->nodes[i].degree);
		for(int j = 0; j < neighbors.size(); ++j) {
			printf("%d -> %d, weight: %f\n", i, neighbors[j].target, neighbors[j].weight);

		}
	}
	return 1;
}