#include <stdio.h>
#include <stdlib.h>
#include <set>

std::set<int> neighborCommunities(node_t* node, int* comm);
float computeDeltaQ(graph_t* graph, node_t currentNode, int currentComm);
void removeNode(graph_t* graph, int currentNodeId);
void addNode(graph_t* graph, int currentNodeId, int newComm);
int phase1(graph_t* graph);