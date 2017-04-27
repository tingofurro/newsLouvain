#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <omp.h>
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
	
	map<int,int> idx2bucket;
	map<int,int> localindex;
	map<keyPair, int> globalindex;
	int steps;
	string filename = "graph/graph0years.txt";

	graph_t* graphs = createMasterGraph("graph/graph0years.txt", &steps, &idx2bucket, &localindex, &globalindex);

	int glob = globalindex[{11, 103}];

	// DIR *dir;
	// struct dirent *ent;
	// std::vector<char*> graph_files;
	// if ((dir = opendir (argv[1])) != NULL) {
	//   /* print all the files and directories within directory */
	//   while ((ent = readdir (dir)) != NULL) {
	//   	if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
	//     	graph_files.push_back(ent->d_name);
	//     }
	//   }
	//   closedir (dir);
	// } else {
	//   /* could not open directory */
	//   perror ("");
	//   return EXIT_FAILURE;
	// }
	// int numthreads;
 //    #pragma omp parallel {
 //        numthreads = omp_get_num_threads();
 //    }

    int** localCommunities = new int*[steps];

	map<keyPair, int> globalComm;
	int globalCount = 0;
	int totalNodes = 0;
	// #pragma omp parallel for 
	for (int i = 0; i < steps; i++) {
		// char full_path[strlen("graph/")+strlen(graph_files[i])];
		// strcpy(full_path, "graph/");
		// strcat(full_path, graph_files[i]);
		// printf("%s\n", full_path);
		int nComms = 0;
		totalNodes += graphs[i].size;
		localCommunities[i] = louvain(&graphs[i], &nComms);
		for(int j = 0; j < nComms; ++j) {
			globalComm[{i, j}] = globalCount;
			globalCount ++;
		}
	}
	graph_t* globalGraph = new graph_t;
	globalGraph->size = globalCount;
	globalGraph->nodes = new node_t[globalCount];

	string line, token, token2;
	ifstream myfile (filename);
	getline(myfile, line);
	int edgeCount = 0;
	while(getline(myfile, line)) {
		stringstream ss(line);
		int i = 0;
		int source_b, target_b;
		int gsource_i, gtarget_i;
		int lsource_i, ltarget_i;
		int lcomm_s, lcomm_t;
		int gcomm_s, gcomm_t;

		while(getline(ss, token, '|')) {
			if(i == 0) {
				gsource_i = atoi(token.c_str());
				source_b = idx2bucket[gsource_i];
				lsource_i = localindex[gsource_i];
				lcomm_s = localCommunities[source_b][lsource_i];
				gcomm_s = globalComm[{source_b, lcomm_s}];
			}
			if(i == 2) {
				stringstream ss2(token);
				while(getline(ss2, token2, ',')) {
					gtarget_i = atoi(token2.c_str());
					target_b = idx2bucket[gtarget_i];
					ltarget_i = localindex[gtarget_i];

					lcomm_t = localCommunities[target_b][ltarget_i];
					gcomm_t = globalComm[{target_b, lcomm_t}];

					int exist = 0;
					float sameRatio = (gcomm_t==gcomm_s)?0.5:1.0;
					// printf("%d %d\n", source_b, target_b);

					for(int k = 0; k < globalGraph->nodes[gcomm_s].neighbors.size(); ++k) {
						if(globalGraph->nodes[gcomm_s].neighbors[k].target == gcomm_t) {
							globalGraph->nodes[gcomm_s].neighbors[k].weight += sameRatio;
							exist = 1;
							break;
						}
					}
					if(exist == 0) {
						edge_t* edgePtr = new edge_t;
						edgePtr->target = gcomm_t; edgePtr->weight = sameRatio; 
						globalGraph->nodes[gcomm_s].neighbors.push_back(*edgePtr);
					}
					globalGraph->nodes[gcomm_s].degree += sameRatio;
					globalGraph->nEdges += sameRatio;
				}

			}
			i ++;
		}
	}
	globalGraph->comm = new int[globalGraph->size];
	for(int i  = 0; i < globalGraph->size; ++i) {globalGraph->comm[i] = i;}
	computeSigmas(globalGraph);

	float* Sin = globalGraph->Sin;
	float* Stot = globalGraph->Stot;
	float totEdges = 0.0;
	
	int nComms;
	int* finalGcomms = louvain(globalGraph, &nComms);
	int* keepTrackComm = new int[totalNodes];

	ofstream outfile;
	outfile.open("comm.out");

	int bucket, localidx, lcomm, gcomm;
	for(int i = 0; i < totalNodes; ++i) {
		bucket = idx2bucket[i];
		localidx = localindex[i];
		lcomm = localCommunities[bucket][localidx];
		gcomm = globalComm[{bucket, lcomm}];

		keepTrackComm[i] = finalGcomms[gcomm];
		outfile << keepTrackComm[i] << endl;

	}
	outfile.close();

}