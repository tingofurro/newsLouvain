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

void removeNode(graph_t* graph, int currentNodeId) {
	node_t currentNode = graph->nodes[currentNodeId];
	int myComm = graph->comm[currentNodeId];
	float* Sin = graph->Sin;
	float* Stot = graph->Stot;

	for(int j = 0; j < currentNode.neighbors.size(); ++j) {
		edge_t currentEdge = currentNode.neighbors[j];
		int currentNeighbor = currentEdge.target;
		int neighborComm = graph->comm[currentNeighbor];
		if(myComm == neighborComm) {
			Sin[myComm] -= currentEdge.weight;
			Stot[myComm] -= currentEdge.weight;
		}
		else {
			Stot[myComm] -= currentEdge.weight;
			Stot[neighborComm] -= currentEdge.weight;
		}
	}
}
void addNode(graph_t* graph, int currentNodeId, int newComm) {
	node_t currentNode = graph->nodes[currentNodeId];
	float* Sin = graph->Sin;
	float* Stot = graph->Stot;
	for(int j = 0; j < currentNode.neighbors.size(); ++j) {
		edge_t currentEdge = currentNode.neighbors[j];
		int currentNeighbor = currentEdge.target;
		int neighborComm = graph->comm[currentNeighbor];
		if(newComm == neighborComm) {
			Sin[newComm] += currentEdge.weight;
			Stot[newComm] += currentEdge.weight;
		}
		else {
			Stot[newComm] += currentEdge.weight;
			Stot[neighborComm] += currentEdge.weight;
		}
	}
	graph->comm[currentNodeId] = newComm;
}
float computeDeltaQ(graph_t* graph, node_t currentNode, int currentComm) {
	float* Sin = graph->Sin;
	float* Stot = graph->Stot;
	float m = graph->nEdges;
	float kiin = 0.0;
	float ki = currentNode.degree;
	int* comm = graph->comm;
	vector<edge_t> neighbors = currentNode.neighbors;

	for(int j = 0; j < neighbors.size(); ++j) {
		if(comm[neighbors[j].target] == currentComm) {
			kiin += neighbors[j].weight;
		}
	}
	float A = (Sin[currentComm] + kiin) / (2*m);
	float B = (Stot[currentComm] + ki)  / (2*m);
	float C = (Sin[currentComm]) / (2*m);
	float D = (Stot[currentComm])  / (2*m);
	float E = (ki)  / (2*m);
	return (A - B*B) - (C - D*D - E*E);
}
int phase1(graph_t* graph) {
	int* comm = graph->comm;
	bool anyChange = true;
	int numChanges = 0;
	int* nodeOrder = new int[graph->size];
	for(int i = 0; i < graph->size; ++i) {nodeOrder[i] = i;}
	
	set<int> comm_set;
	set<int>::iterator it;

	while(anyChange) {
		// printf("New full phase 1 is starting %d\n", numChanges);
		anyChange = false;
		random_shuffle(&nodeOrder[0], &nodeOrder[graph->size]);
		random_shuffle(&nodeOrder[0], &nodeOrder[graph->size]);
		random_shuffle(&nodeOrder[0], &nodeOrder[graph->size]);
		random_shuffle(&nodeOrder[0], &nodeOrder[graph->size]);
		// will start with just doing node 0
		for(int i = 0; i < graph->size; i++) {
			int currentNodeId = nodeOrder[i];
			// printf("Considering node %d\nPotential communities: ", currentNodeId);
			node_t currentNode = graph->nodes[currentNodeId];
			comm_set = neighborCommunities(&currentNode, comm);
			int oldComm = comm[currentNodeId];
			comm_set.insert(oldComm); // Add my own community to the choices
			
			// Update the Sigmas to be without that node
			removeNode(graph, currentNodeId);

			float maxDeltaQ = -0.01; int maxComm = -1;
			for (it = comm_set.begin(); it != comm_set.end(); ++it) {
				int currentComm = *it;
				float deltaQ = computeDeltaQ(graph, currentNode, currentComm);
				if(deltaQ > maxDeltaQ) {
					maxDeltaQ = deltaQ; maxComm = currentComm;
				}
			}

			// printf("%d: maxDeltaQ: %.2f %d %d \n", currentNodeId, maxDeltaQ, oldComm, maxComm);

			addNode(graph, currentNodeId, maxComm);
			// print_sigmas(graph);
			anyChange = anyChange || (maxComm != oldComm);
			
			if(maxComm != oldComm) {numChanges ++;}
			// printf("My old comm was: %d, my new comm is %d, the change: %d\n", oldComm, maxComm, (anyChange?1:0));
			// printf("-------------------\n");
		}
	}
	return numChanges;
}