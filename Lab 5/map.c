#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "matrices.h"

extern pthread_mutex_t lock;

/*matrices.c*/


//update matrix (map function)
void new_map_data(int** Map, int* data){
	//when dealing with cost table, remember to lock it first
	pthread_mutex_lock(&lock);

	//set both directions: from src and to src to cost 
	Map[data[0]][data[1]] = data[2];
	Map[data[1]][data[0]] = data[2];

	pthread_mutex_unlock(&lock);

	printf("\nNew map data obtained from Operator:\n");
	pmatrix(Map);

}


/* prints the matrix */
void pmatrix(int** Map)
{
	//when dealing with cost table, remember to lock it first
	pthread_mutex_lock(&lock);

	for(int row = 0; row < 4; row++)
	{
		printf("| %d %d %d %d |\n", Map[row][0], Map[row][1], Map[row][2], Map[row][3]);
	}

	//unlock
	pthread_mutex_unlock(&lock);
}

//function that parses the cost matrix from file
//returns cost table 
int** init_matrix(FILE *cost)
{
	//whenever u make a matrix, u malloc
	//so matrix still exists outside of fxn
	int **matrix = (int **)malloc(4*sizeof(int*));

	//allocate mem space for inner array in adjacency matrix
	for (int i = 0; i < 4; i++){
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

