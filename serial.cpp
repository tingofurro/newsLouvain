#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <omp.h>
#include "graph.cpp"
#include "louvain.cpp"
#include <set>
#include <vector>


int louvain(char* file_name) {
	printf("%s\n", "creating graphs");
	graph_t* graph = createGraphFromFile(file_name);
	printf("%s\n", "done creating graphs");
	graph_t* original_graph = graph;
	int* keepTrackComm = new int[graph->size];
	for(int i = 0; i < graph->size; ++i) {keepTrackComm[i] = i;}

	int numChanges = 1;
	while(numChanges > 0) {
		printf("Phase. Nodes: %d \n", graph->size);
		numChanges = phase1(graph);
		compactComm(graph);

		for(int i  = 0; i < original_graph->size; ++i) {
			keepTrackComm[i] = graph->comm[keepTrackComm[i]];
		}
		printf("Before merge\n");
		//printf("number of edges is %f\n", graph->nEdges);
		graph_t* newGraph = mergeGraph(graph);
		graph = newGraph;
		printf("Got here merge\n");
		//printf("number of edges is %f\n", graph->nEdges);
	}

	// for(int i = 0; i < original_graph->size; ++i) {
	// 	printf("%d\n", keepTrackComm[i]);
	// }
	return 1;
}

int main(int argc, char **argv) {
	DIR *dir;
	struct dirent *ent;
	std::vector<char*> graph_files;
	if ((dir = opendir (argv[1])) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
	  	if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
	    	graph_files.push_back(ent->d_name);
	    }
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("");
	  return EXIT_FAILURE;
	}
	
	#pragma omp parallel for 
	for (int i = 0; i < graph_files.size(); i++) {
		
		char full_path[strlen("graph/")+strlen(graph_files[i])];
		strcpy(full_path, "graph/");
		strcat(full_path, graph_files[i]);
		printf("%s\n", full_path);
		louvain(full_path);
	}
}
