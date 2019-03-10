#ifndef MATRICES_H
#define MATRICES_H

#include <pthread.h>
#include <stdio.h>

//cost table (matrix and the mutex)
typedef struct {
	int ** matrix; // cost table whose changes have been made 

	pthread_mutex_t* lock;
} Map; 

//function that parses the cost matrix from file
//returns cost table 
int** init_matrix(FILE *fp);

//function that prints the matrix
void pmatrix(int sz, int matrix[sz][sz]);

//dijkstra's algorithm implementation
//passes in a costs table and the source (machine who)
void nier(int matrix[V][V], int src);

#endif