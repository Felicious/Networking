#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "matrices.h"

/*matrices.c*/

/* prints the matrix */
void pmatrix(Map* mrix)
{
	//when dealing with cost table, remember to lock it first
	int** matrix = pthread_mutex_lock(mrix->lock);

	for(int row = 0; row < sz; row++)
	{
		printf("| %d %d %d %d |\n", matrix[row][0], matrix[row][1], matrix[row][2], matrix[row][3]);
	}

	//unlock
	pthread_mutex_unlock(mrix->lock);
}

//function that parses the cost matrix from file
//returns cost table 
int** init_matrix(FILE *cost)
{
	//whenever u make a matrix, u malloc
	//so matrix still exists outside of fxn
	int **matrix = (int **)malloc(4*sizeof(int*));

	//allocate mem space for inner array in adjacency matrix
	for (int i = 0; i< 4; i++){
		matrix[i] = (int **)malloc(4*sizeof(int*));
	}

	//read file and read values into 
	for(int row = 0; row < N; row++)
	{
		fscanf(cost, "%d %d %d %d", &matrix[row][0], &matrix[row][1], &matrix[row][2], &matrix[row][3]);
	}

	fclose(cost);

	return matrix;
}