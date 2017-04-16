#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 50
#endif

struct neighbor {
	int node;
	int weight;
};

typedef struct neighbor neighbor_t;

struct bucket {
   neighbor_t *neighbors;          // Pointer to the first entry of that bucket
   int neighborsCount;
};

typedef struct bucket bucket_t;

struct hashtable {
	int size;
	bucket_t* buckets;
}

typedef struct hashtable hashtable_t;

void getDegrees(hashtable_t* table, int* degrees, int n) {
	for (int i = 0; i < n; i++) {
		neighbor_t* neighbors = (table->buckets[i]).neighbors;
		int neighborsCount = (table->buckets[i]).neighborsCount;
		int degree = 0;
		for (int j = 0; j < neighborsCount; j++) {
			degree += neighbors[j].weight;
		}
		degrees[i] = degree;
	}
}

int createGraphFromFile(const char *filename, hashtable_t* table) {
	FILE *f = fopen(filename, "r");
	int n;
	fscanf(file, "%d", &i);  
	unsigned char *working_buffer;
	working_buffer = (unsigned char*) malloc(MAX_LINE_SIZE * sizeof(unsigned char));
	fgets(working_buffer, MAX_LINE_SIZE, f);
	printf("%s\n", working_buffer);
}
