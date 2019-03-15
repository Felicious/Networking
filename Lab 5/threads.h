#ifndef THREADS_H
#define THREADS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "automata.h"
#include "nier.h"
#include "map.h"


extern int machine_id;
extern int map_database[4][4];
// our node (called automata)object
extern pthread_mutex_t lock;



/*
THREAD # 1
Receive thread

Repeats forever, receiving msg from other nodes &
update its neighbor's cost table. Updates cost in
both directions, cost c from src to dest and dest 
to src

It's called A2 because in the game, Nier Automata,
A2 is one of the first gen automata sent to Earth &
in this function sets up the connection btw the 
nodes and is otherwise responsible for the majority
of initialization procedures for est. connections

*/
void* A2(void* arg);

//helper function for thread 2
int Pod_042(int *map, int i);

/*
THREAD # 2
Sender thread

Called 2B because she perpetuates the story in Nier
Automata, even following her death ):

*/
void TwoB();


/*
THREAD # 3
Updater thread

Repeats forever, following random delays, runs the
algo to update the least path

It's called NineS because 9S supports 2B in Nier, &
the updater thread updates and maintains the matrix
to provide useful info for the algorithm's use.
*/
void* NineS(void* arg);

#endif