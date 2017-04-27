#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "graph.cpp"
#include "louvain.cpp"
#include <set>
#include <vector>
#include <map>

int* louvain(graph_t* graph, int* nComms) {
	graph_t* original_graph = graph;
	int* keepTrackComm = new int[graph->size];
	for(int i = 0; i < graph->size; ++i) {keepTrackComm[i] = i;}
	int numChanges = 1;
	while(numChanges > 0) {
		numChanges = phase1(graph);
		compactComm(graph);

		for(int i  = 0; i < original_graph->size; ++i) {
			keepTrackComm[i] = graph->comm[keepTrackComm[i]];
		}
		graph_t* newGraph = mergeGraph(graph);
		graph = newGraph;
		*nComms = newGraph->size;
	}
	return keepTrackComm;
}

int main(int argc, char **argv) {
	
	graph_t* serialGraph = createGraph("graph/graph0years.txt");
	int* nComms = new int;
	int* keepTrackComm = louvain(serialGraph, nComms);
	cout << *nComms << endl;

	ofstream outfile;
	outfile.open("comm-serial.out");
	for(int i = 0; i < serialGraph->size; ++i) {
		outfile << keepTrackComm[i] << endl;

	}
	outfile.close();

}