#include "louvain.h"

std::set<int> neighborCommunities(node_t* node, int* comm) {
	vector<edge_t> neighbors = node->neighbors;
	std::set<int> comm_set;
	for(int j = 0; j < neighbors.size(); ++j) {
		comm_set.insert(comm[neighbors[j].target]);
	}

	return comm_set;
}