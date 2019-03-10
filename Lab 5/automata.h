#ifndef AUTOMATA_H
#define AUTOMATA_H

// #include thread
#include "matrices.h"


// our node (called automata)object
typedef struct {
	int index; //node's index 
	char name[10];
	char ip_addr[50];
	int port_no;
} Automata; 

//between each of our automata, we have shared connection of matrices
typedef struct {
	Automata* automaton; //current automata

	Automata* automata; //all automata

	int* sleep; //whether or not to sleep

	

}
