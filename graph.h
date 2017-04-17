#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 1000
#endif

typedef struct neighbor neighbor_t;
struct neighbor {
	int node;
	int weight;
	neighbor_t* next;
};



struct bucket {
   neighbor_t *head;          // Pointer to the first entry of that bucket
   int neighborsCount;
};

typedef struct bucket bucket_t;

struct hashtable {
	int size;
	bucket_t* buckets;
};

typedef struct hashtable hashtable_t;

// void getDegrees(hashtable_t* table, int* degrees, int n) {
// 	for (int i = 0; i < n; i++) {
// 		neighbor_t* neighbors = (table->buckets[i]).head;
// 		int neighborsCount = (table->buckets[i]).neighborsCount;
// 		int degree = 0;
// 		for (int j = 0; j < neighborsCount; j++) {
// 			degree += neighbors[j].weight;
// 		}
// 		degrees[i] = degree;
// 	}
// }

void createGraphFromFile(const char *filename, hashtable_t* table) {
	FILE *f = fopen(filename, "r");
	char working_buffer[MAX_LINE_SIZE];
	char* source;
	char* targetList;
	char* target;
	int n;
	fgets(working_buffer, sizeof(working_buffer), f);
	n = atoi(working_buffer);//, working_buffer+MAX_LINE_SIZE, 10);
	table-> size = n;
	table-> buckets = (bucket_t*) malloc(sizeof(bucket_t)*n);
	while (fgets(working_buffer, sizeof(working_buffer), f) != NULL) {

		source = strtok(working_buffer, ":");
		int source_index = atoi(source);//strtol(source, source+MAX_LINE_SIZE, 10);

		targetList = strtok(NULL, ":");
		target = strtok(targetList, ",");
		neighbor_t* neighbor;
		neighbor = (neighbor_t*) malloc(sizeof(neighbor_t));
		neighbor->node = atoi(target);//, target+MAX_LINE_SIZE, 10);
		neighbor->weight = 1;
		(table->buckets)[source_index].head = neighbor;
		(table->buckets)[source_index].head->next = NULL;
		int neighborsCount = 1;
		while((target = strtok(NULL, ",")) != NULL) {
			neighbor = (neighbor_t*) malloc(sizeof(neighbor_t));
			neighbor->node = atoi(target);//, target+MAX_LINE_SIZE, 10);
			neighbor->weight = 1;
			neighbor->next = (table->buckets)[source_index].head;
			(table->buckets)[source_index].head = neighbor;
			
			neighborsCount ++;			
		}
		(table->buckets)[source_index].neighborsCount = neighborsCount;
	}
	
}
