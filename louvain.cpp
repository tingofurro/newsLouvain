#include "louvain.h"
#include <algorithm>

set<int> neighborCommunities(node_t* node, int *comm) {
	vector<edge_t> neighbors = node->neighbors;
	set<int> comm_set;
	for(int j = 0; j < neighbors.size(); ++j) {
		comm_set.insert(comm[neighbors[j].target]);
	}
	return comm_set;
}
void buildCommunitySets(graph_t* graph) {
	// This assumes that graph->nComm is correct

	graph->commSets = new set<int>[graph->nComm];
	int* comm = graph->comm;
	for(int i = 0; i < graph->size; ++i) {graph->commSets[comm[i]].insert(i);}

	// set<int>::iterator it;
	// for(int i = 0; i < graph->nComm; ++i) {
	// 	printf("---------------\n");
	// 	for (it = graph->commSets[i].begin(); it != graph->commSets[i].end(); ++it) {
	// 		printf("%d\n", *it);
	// 	}
	// }
}
void computeSigmas(graph_t* graph) {
	// This assumes graph->commSets is correct
	set<int> comm_set;
	graph->Sin = new float[graph->nComm];
	graph->Stot = new float[graph->nComm];

	int* comm = graph->comm;
	float* Sin = graph->Sin;
	float* Stot = graph->Stot;

	set<int>::iterator it; node_t currentNode;
	for(int i = 0; i < graph->nComm; ++i) {
		set<int> commSet = graph->commSets[i];
		Sin[i] = 0; Stot[i] = 0;
		for (it = graph->commSets[i].begin(); it != graph->commSets[i].end(); ++it) {
			currentNode = graph->nodes[*it];
			Stot[i] += currentNode.degree;
			for(int j = 0; j < currentNode.neighbors.size(); ++j) {
				int currentNeighbor = currentNode.neighbors[j].target;
				if(commSet.find(currentNeighbor) != commSet.end()) {
					Sin[i] += 0.5*currentNode.neighbors[j].weight; // This is because edges will be double counted
					Stot[i] -= 0.5*currentNode.neighbors[j].weight; // Remove the excess
				}
			}
		}
	}
}
void phase1(graph_t* graph) {
	int* comm = graph->comm;
	int* nodeOrder = (int*) malloc(graph->size*sizeof(int));
	for(int i = 0; i < graph->size; ++i) {nodeOrder[i] = i;}

	random_shuffle(&nodeOrder[0], &nodeOrder[graph->size]);
	std::set<int> comm_set;

	for(int i = 0; i < graph->size; i++) {
		int currentNode = nodeOrder[i];
		printf("Currently at node %d\n", currentNode);
		comm_set = neighborCommunities(&(graph->nodes[currentNode]), graph->comm);
		comm_set.insert(comm[currentNode]); // Add my own community to the choices
		while(!comm_set.empty()) {
			int currentSwitch = *comm_set.begin();
			printf("%d\n", currentSwitch);
			comm_set.erase(comm_set.begin());
		}
		printf("================================\n");
	}
}