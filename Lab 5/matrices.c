#include <stdio.h>
#include "matrices.h"

/*matrices.c*/

/* prints the matrix */
void pmatrix(int sz, int matrix[sz][sz])
{
	for(int row = 0; row < sz; row++)
	{
		printf("| %d %d %d %d |\n", matrix[row][0], matrix[row][1], matrix[row][2], matrix[row][3]);
	}
}


void init_matrix(int N, int matrix[N][N], char *argv)
{
	FILE *cost;

	cost = fopen(argv, "rb");
	if(!cost){
		printf("File cannot be opened\n");
		exit(1);
	}

	for(int row = 0; row < N; row++)
	{
		fscanf(cost, "%d %d %d %d", &matrix[row][0], &matrix[row][1], &matrix[row][2], &matrix[row][3]);
	}
}