/*****************************
 * COEN 146, UDP, client
 *****************************/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "automata.h"
#include "nier.h"
#include "map.h"
#include "threads.h"


// Global variables
pthread_mutex_t lock;

typedef struct {
	char name[50];
	char ip_addr[50];
	int port_no;
}Automata; 

int host_no;
int machine_id;

Automata automata[4];
//global cost matrix
int map_database[4][4];


/***********
 *  main
 ***********/
int main (int argc, char *argv[])
{
	perror("Top of the program!\n");

	pthread_mutex_init(&lock,NULL);

/*
	if (argc < 5)
	{
		printf ("parameters: %s <machine_id> <num_of_machines> <costs_file> <machines_file>\n");
		return 1;
	}
*/
	
	//make an empty adjacency matrix to store our cost matrix
	FILE fp = fopen(argv[3], "r");
	if(fp == NULL){
		printf("Problem opening cost matrix file!\n");
		return 0;
	}
	
	//copies values from file into global map_database
	init_matrix(fp); 

	

	machine_id = atoi(argv[1]);

	FILE fpp = fopen(argv[4], "r");
	



	




	int cost_matrix[4][4]; //this is the neighbor cost matrix

	int flag = 1;
	init_matrix(4, cost_matrix, argv[1], flag);
	if(flag == 0)
		return 0;






	

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