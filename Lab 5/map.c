#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"

extern pthread_mutex_t lock;

/*matrices.c*/


//update matrix (map function)
void new_map_data(int* data){
	//when dealing with cost table, remember to lock it first
	pthread_mutex_lock(&lock);

	//set both directions: from src and to src to cost 
	map_database[data[0]][data[1]] = data[2];
	map_database[data[1]][data[0]] = data[2];

	pthread_mutex_unlock(&lock);

	printf("\nNew map data obtained from Operator:\n");
	pmatrix();

}


/* prints the matrix */
void pmatrix()
{
	//when dealing with cost table, remember to lock it first
	pthread_mutex_lock(&lock);

	for(int row = 0; row < 4; row++)
	{
		printf("| %d %d %d %d |\n", map_database[row][0], map_database[row][1], map_database[row][2], map_database[row][3]);
	}

	//unlock
	pthread_mutex_unlock(&lock);
}

//function that parses the cost matrix from file
//returns cost table 
void init_matrix(FILE *cost)
{
	if(!cost){
		printf("Problem opening matrix gile\n");
		return;
	}

	//read file and read values into 
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++)
			fscanf(cost, "%d", &map_database[row][col]);
	}

	printf("Initialized cost matrix: \n");
	pmatrix();

	fclose(cost);
}

