#include <stdio.h>
#include "matrices.h"

/*matrices.c*/

/* prints the matrix */
void pmatrix(int matrix[int sz][int sz], int sz)
{
	for(int row = 0; row < sz; row++)
	{
		printf("| %d %d %d %d |\n", matrix[row][0], matrix[row][1], matrix[row][2], matrix[row][3]);
	}
}


void init_matrix(int matrix[int N][int N], int N)
{
	FILE *cost;

	cost = fopen(argv[1], "rb");
	if(!cost){
		printf("File cannot be opened\n");
		return 0;
	}

	for(int row = 0; row < N; row++)
	{
		fscanf(cost, "%d %d %d %d", &matrix[row][0], &matrix[row][1], &matrix[row][2], &matrix[row][3]);
	}
}