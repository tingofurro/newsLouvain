#include "graph.h"

graph_t* createGraphFromFile(const char *filename) {
	graph_t * graph = (graph_t *) malloc(sizeof(graph_t));

	FILE *f = fopen(filename, "r");
	char working_buffer[MAX_LINE_SIZE];
	char* source;
	char* targetList;
	char* target;
	int n;
	fgets(working_buffer, sizeof(working_buffer), f);
	n = atoi(working_buffer);
	graph->size = n;
	graph->nodes = (node_t*) malloc(sizeof(node_t)*n);
	printf("%d\n", n);
	while (fgets(working_buffer, sizeof(working_buffer), f) != NULL) {

		source = strtok(working_buffer, ":");
		int source_index = atoi(source);

		targetList = strtok(NULL, ":");
		target = strtok(targetList, ",");
		edge_t* edgePtr = (edge_t*) malloc(sizeof(edge_t));
		edgePtr->target = atoi(target); edgePtr->weight = 1.0; 
		graph->nodes[source_index].neighbors.push_back(*edgePtr);
		int degree = 1;
		while((target = strtok(NULL, ",")) != NULL) {
			edgePtr = (edge_t*) malloc(sizeof(edge_t));
			edgePtr->target = atoi(target); edgePtr->weight = 1.0; 
			graph->nodes[source_index].neighbors.push_back(*edgePtr);

			degree ++;			
		}
		(graph->nodes)[source_index].degree = degree;
		(graph->nEdges) += 0.5*degree;
	}

	// Initialize the communities
	// For the graph we were playing with
	graph->comm = new int[graph->size];
	for(int i  = 0; i < graph->size; ++i) {
		if(i == 0) {graph->comm[i] = 0;}
		if(i == 1 || i == 4 || i == 7 || i == 8) {graph->comm[i] = 1;}
		if(i == 2 || i == 3 || i == 5 || i == 6) {graph->comm[i] = 2;}
	}

	// for(int i  = 0; i < graph->size; ++i) {graph->comm[i] = i;}
	computeSigmas(graph);

	return graph;
}
void computeSigmas(graph_t* graph) {
	graph->Sin = new float[graph->size];
	graph->Stot = new float[graph->size];

	int* comm = graph->comm;
	float* Sin = graph->Sin;
	float* Stot = graph->Stot;

	for(int i = 0; i < graph->size; ++i) {
		Sin[i] = 0.0; Stot[i] = 0.0;
	}

	node_t currentNode;
	int currentComm = currentComm;
	for(int currentNodeId = 0; currentNodeId < graph->size; currentNodeId++) {
		currentNode = graph->nodes[currentNodeId];
		currentComm = comm[currentNodeId];
		Stot[currentComm] += currentNode.degree;
		for(int j = 0; j < currentNode.neighbors.size(); ++j) {
			int currentNeighborId = currentNode.neighbors[j].target;
			if(comm[currentNeighborId] == currentComm) {
				Sin[currentComm] += 0.5*currentNode.neighbors[j].weight; // This is because edges will be double counted
				Stot[currentComm] -= 0.5*currentNode.neighbors[j].weight; // Remove the excess total counts
			}
		}
	}
	// for(int i = 0; i < graph->size; ++i) {
	// 	printf("Sin[%d] = %.2f, Stot[%d] = %.2f\n", i, Sin[i], i, Stot[i]);
	// }
}
graph_t* compactComm(graph_t* graph) {
	int* comm = graph->comm;
	// We merge back the communities into a good range
	int* newCommNbs = new int[graph->size]; // This is an upperbound... yolo
	std::set<int> comm_set;
	int o = 0;
	for (int i = 0; i < graph->size; ++i) {
		if(comm_set.find(comm[i]) == comm_set.end()) {
			comm_set.insert(comm[i]);
			newCommNbs[comm[i]] = o;
			o ++;
		}
	}

	for (int i = 0; i < graph->size; ++i) {
		graph->comm[i] = newCommNbs[(graph->comm[i])];
	}

	delete [] newCommNbs;
}
graph_t* mergeGraph(graph_t* graph) {
	int* comm = graph->comm;
	graph_t* newGraph = (graph_t*) malloc(sizeof(graph_t));
	std::set<int> comm_set;
	for (int i = 0; i < graph->size; ++i) {comm_set.insert(comm[i]);}
	newGraph->size = comm_set.size();
	newGraph->nodes = (node_t*) malloc(comm_set.size() * sizeof(node_t));
	for (int i = 0; i < graph->size; ++i) {
		int cur_comm = comm[i];
		std::vector<edge_t> old_neighbors = graph->nodes[i].neighbors;
		
		for(int j = 0; j < old_neighbors.size(); ++j) {

			int neighbor_comm = comm[old_neighbors[j].target];
			int exist = 0;
			float ratio = (neighbor_comm==cur_comm)?0.5:1.0;
			for (int k = 0; k < newGraph->nodes[cur_comm].neighbors.size(); ++k) {
				if (newGraph->nodes[cur_comm].neighbors[k].target == neighbor_comm) {
					newGraph->nodes[cur_comm].neighbors[k].weight += ratio*old_neighbors[j].weight;
					exist = 1;
					break;
				}
			}
			// edge does not yet exist
			if (exist == 0) {
				edge_t *edgePtr = (edge_t*) malloc(sizeof(edge_t));
				edgePtr->target = neighbor_comm; edgePtr->weight = ratio*old_neighbors[j].weight; 
				newGraph->nodes[cur_comm].neighbors.push_back(*edgePtr);
			}
			newGraph->nodes[cur_comm].degree += old_neighbors[j].weight;
			newGraph->nEdges += 0.5*old_neighbors[j].weight;
		}
	}
	// for(int i = 0; i < newGraph->size; ++i) {
	// 	printf("-----------------------\n");
	// 	vector<edge_t> neighbors = newGraph->nodes[i].neighbors;
	// 	for(int j = 0; j < neighbors.size(); ++j) {
	// 		printf("%d -> %d = %2.f\n", i, neighbors[j].target, neighbors[j].weight);
	// 	}
	// }
	newGraph->comm = new int[newGraph->size];
	for(int i  = 0; i < newGraph->size; ++i) {newGraph->comm[i] = i;}
	computeSigmas(newGraph);

	return newGraph;
}