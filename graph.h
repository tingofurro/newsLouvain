#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <vector>

#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 1000
#endif

using namespace std;
struct edge {
	int target;
	float weight;
};
typedef struct edge edge_t;

struct node {
	vector<edge_t> neighbors;
	int degree;
};

typedef struct node node_t;

struct graph {
	int size;
	node_t* nodes;
};

typedef struct graph graph_t;

graph_t* createGraphFromFile(const char *filename);