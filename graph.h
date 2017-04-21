#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <set>

#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 500
#endif

using namespace std;
struct edge {
	int target;
	float weight;
};
typedef struct edge edge_t;

struct node {
	vector<edge_t> neighbors;
	float degree;
};

typedef struct node node_t;

struct graph {
	int size;
	node_t* nodes;

	float nEdges;
	int* comm; // 0, 1, 2, 2, 1, 2, 2, 1, 1
	// set<int> *commSets; // 0->[0], 1->[1,4,7,8], 2->[2,3,5,6]

	float* Sin;
	float* Stot;
};

typedef struct graph graph_t;

graph_t* createGraphFromFile(const char *filename);
void computeSigmas(graph_t* graph);
graph_t* mergeGraph(graph_t* graph);