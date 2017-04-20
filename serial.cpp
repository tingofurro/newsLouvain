#include <stdlib.h>
#include <stdio.h>
#include "graph.cpp"
#include "louvain.cpp"
#include <set>

int main(int argc, char **argv) {

	graph_t* graph = createGraphFromFile(argv[1]);

	int comm[] = {0,6,1,1,2,2,3,3, 4, 5, 6, 7, 8};

	std::set<int> comm_set = neighborCommunities(&graph->nodes[0], comm);
	graph_t* newGraph = mergeGraph(graph, comm);

	while(!comm_set.empty()) {
		printf("%d\n", *comm_set.begin());
		comm_set.erase(comm_set.begin());
	}

	for (int i = 0; i < newGraph->size; i++) {
		vector<edge_t> neighbors = newGraph->nodes[i].neighbors;
		printf("Degree of node[%d] = %d\n", i, newGraph->nodes[i].degree);
		for(int j = 0; j < neighbors.size(); ++j) {
			printf("%d -> %d\n", i, neighbors[j].target);
		}
	}
	return 1;
}