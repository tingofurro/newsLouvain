#include "louvain.h"
std::set<int> neighborCommunities(node_t* node, int* comm) {
	vector<edge_t> neighbors = node->neighbors;
	std::set<int> comm_set;
	for(int j = 0; j < neighbors.size(); ++j) {
		comm_set.insert(comm[neighbors[j].target]);
	}

	return comm_set;
}


graph_t* mergeGraph(graph_t* graph, int* comm) {
	graph_t* newGraph = (graph_t*) malloc(sizeof(graph_t));
	std::set<int> comm_set;
	for (int i = 0; i < graph->size; ++i) {
		comm_set.insert(comm[i]);
	}
	newGraph->size = comm_set.size();
	newGraph->nodes = (node_t*) malloc(comm_set.size() * sizeof(node_t));
	printf("comm_set size: %d\n", newGraph->size);
	for (int i = 0; i < graph->size; ++i) {
		int cur_comm = comm[i];
		std::vector<edge_t> cur_neighbors = newGraph->nodes[cur_comm].neighbors;
		std::vector<edge_t> old_neighbors = graph->nodes[i].neighbors;
		
		std::vector<int> cur_neighbors_nodes;
		for (int j = 0; j < cur_neighbors.size(); j++) {
			cur_neighbors_nodes.push_back(cur_neighbors[j].target);
		}

		for(int j = 0; j < old_neighbors.size(); ++j) {
			int old_neighbor_comm = comm[old_neighbors[j].target];
			int exist = 0;
			for (int k = 0; k < cur_neighbors_nodes.size(); ++k) {
				if (cur_neighbors_nodes[k] == old_neighbor_comm) {
					cur_neighbors[k].weight += old_neighbors[j].weight;
					exist = 1;
				}
			}
			// edge does not yet exist
			if (exist == 0) {
				edge_t *edgePtr = (edge_t*) malloc(sizeof(edge_t));
				edgePtr->target = old_neighbor_comm; edgePtr->weight = old_neighbors[j].weight; 
				cur_neighbors.push_back(*edgePtr);
				newGraph->nodes[i].degree++;
				printf("need to push %d to %d's neighbor list\n", old_neighbor_comm, cur_comm); 

			}
		}
	}
	return newGraph;
}