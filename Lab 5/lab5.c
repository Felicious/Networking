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
	automata_config(fpp);
	
	srand(time(NULL));

	printf("time to spawn threads! \n");

	pthread_t receiver_A2;
	printf("creating A2 receiver thread");
    pthread_create(&receiver_A2, NULL, A2, NULL);

	pthread_t updater_9S;
	pthread_create(&updater_9S, NULL, NineS, NULL);

	//read the changes 
	TwoB();

	//join threads together 
	// join other threads
    if (pthread_join(receiver_A2, NULL)) {
        printf("Failed to join receiver: A2\n");
        return 1;
    }

    if (pthread_join(updater_9S, NULL)) {
        printf("Failed to join updater: 9S\n");
        return 1;
    }
	
	return 0;
}
