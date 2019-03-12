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
	
	//copies values from file into global map_database
	init_matrix(fp); 

	
	char* end; //used to check if conversion failed
	machine_id = strtol(argv[1], &end, 10);
	if(machine_id > 3 || end <= argv[1]){
		printf("Machine ID should be between 0 and 3 ):\n");
		return 0;
	}

	//create automatas
	FILE fpp = fopen(argv[4], "r");
	
	srand(time(NULL));

	printf("time to spawn threads! \n");

	pthread_t receiver_A2;
	printf("creating A2 receiver thread");
    pthread_create(&receiver_A2, NULL, A2, NULL);

	pthread_t updater_9S;


	int data[3]; //this is the cost update 
	data[0] = machine_id;
	//identify which machine is running
	printf("Machine %d Initialized\n", machine_id);
	while(1){
		
		sleep(10);
	}







	

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