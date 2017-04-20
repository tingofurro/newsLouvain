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
		(graph->nEdges) += degree;
	}
	return graph;
}