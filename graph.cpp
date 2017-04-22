#include "graph.h"

graph_t* createGraphFromFile(const char *filename) {
	graph_t * graph = (graph_t *) malloc(sizeof(graph_t));

	FILE *f = fopen(filename, "r");
	char working_buffer[MAX_LINE_SIZE];
	printf("%s for %s\n", "Initialized working_buffer", filename);
	char* source;
	char* targetList;
	char* target;
	int n;
	fgets(working_buffer, sizeof(working_buffer), f);
	printf("%s\n", "read first line");
	n = atoi(working_buffer);
	graph->size = n;
	graph->nodes = (node_t*) malloc(sizeof(node_t)*n);
	while (fgets(working_buffer, sizeof(working_buffer), f) != NULL) {
		//printf("%s\n", "read another line");
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
	// for(int i  = 0; i < graph->size; ++i) {
	// 	if(i == 0) {graph->comm[i] = 0;}
	// 	if(i == 1 || i == 4 || i == 7 || i == 8) {graph->comm[i] = 1;}
	// 	if(i == 2 || i == 3 || i == 5 || i == 6) {graph->comm[i] = 2;}
	// }

	for(int i  = 0; i < graph->size; ++i) {graph->comm[i] = i;}
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
				float ratio = (currentNeighborId != currentNodeId)?0.5:1.0;
				Sin[currentComm] += ratio*currentNode.neighbors[j].weight; // This is because edges will be double counted
				// Stot[currentComm] -= ratio*currentNode.neighbors[j].weight; // Remove the excess total counts
			}
		}
	}
	// for(int i = 0; i < graph->size; ++i) {
	// 	printf("Sin[%d] = %.2f, Stot[%d] = %.2f\n", i, Sin[i], i, Stot[i]);
	// }
}
void print_graph(graph_t* graph) {
	printf("Printing graph. %.2f\n", graph->nEdges);
	for(int i = 0; i < graph->size; ++i) {
		printf("--- Node %d (deg = %.2f)\n", i, graph->nodes[i].degree);
		vector<edge_t> neighbors = graph->nodes[i].neighbors;
		for(int j = 0; j < neighbors.size(); ++j) {
			printf("%d -> %d : %.2f\n", i, neighbors[j].target, neighbors[j].weight);
		}
	}
}

void print_sigmas(graph_t* graph) {
	float* Sin = graph->Sin;
	float* Stot = graph->Stot;
	float totEdges = 0.0;
	// printf("=======\n");
	for(int i = 0; i < graph->size; ++i) {
		totEdges += Sin[i] + 0.5*(Stot[i]-Sin[i]);
		printf("Sin[%d] = %.2f, Stot[%d] = %.2f\n", i, Sin[i], i, Stot[i]);
	}
	printf("%.2f\n", totEdges);
}


void compactComm(graph_t* graph) {
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
	newGraph->nodes = new node_t[newGraph->size];

	for(int i = 0; i < newGraph->size; ++i) {
		newGraph->nodes[i].degree = 0.0;
	}

	for (int i = 0; i < graph->size; ++i) {
		int cur_comm = comm[i];
		std::vector<edge_t> old_neighbors = graph->nodes[i].neighbors;
		
		for(int j = 0; j < old_neighbors.size(); ++j) {
			int neighbor_comm = comm[old_neighbors[j].target];
			int exist = 0;

			float ratio = (neighbor_comm==cur_comm && i != old_neighbors[j].target)?0.5:1.0;
			for (int k = 0; k < newGraph->nodes[cur_comm].neighbors.size(); ++k) {
				if (newGraph->nodes[cur_comm].neighbors[k].target == neighbor_comm && cur_comm < newGraph->nodes[cur_comm].neighbors[k].target) {
					newGraph->nodes[cur_comm].neighbors[k].weight += old_neighbors[j].weight;
					exist = 1;
					break;
				}
			}
			// edge does not yet exist
			if (exist == 0) {
				edge_t* edgePtr = new edge_t;
				edgePtr->target = neighbor_comm; edgePtr->weight = old_neighbors[j].weight; 
				newGraph->nodes[cur_comm].neighbors.push_back(*edgePtr);
			}
			float ratio2 = (i != old_neighbors[j].target)?0.5:1.0;

			newGraph->nodes[cur_comm].degree += ratio*old_neighbors[j].weight;

			newGraph->nEdges += ratio2*old_neighbors[j].weight;
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
	// print_graph(newGraph);

	computeSigmas(newGraph);
	// print_sigmas(newGraph);
	return newGraph;
}