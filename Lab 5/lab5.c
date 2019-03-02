/*****************************
 * COEN 146, UDP, client
 *****************************/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "helper_fxns.h"

/***********
 *  main
 ***********/
int main (int argc, char *argv[])
{
	perror("Top of the program!\n");

/*
	if (argc < 5)
	{
		printf ("parameters: %s <machine_id> <num_of_machines> <costs_file> <machines_file>\n");
		return 1;
	}
*/
	//parse the costs file 
	FILE *cost;

	cost = fopen(argv[1], "rb");
	if(!cost){
		printf("File cannot be opened\n");
		return 0;
	}

	int matrix[4][4];

	for(int row = 0; row < 4; row++)
	{
		printf(" %d\n",row);
		fscanf(cost, "%d %d %d %d", &matrix[row][0], &matrix[row][1], &matrix[row][2], &matrix[row][3]);
	}

	pmatrix(4, matrix);

	//done parsing



	

	return 0;
}
/*
//parse the costs file 
fopen
scanf
//initialize matrix


//send msgs using array of strings size 3
Thread 2 calls receive from multiple times and update cost matrix

Thread 3 link state algo
go through matrix and compute least dist 
*/