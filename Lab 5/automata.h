#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[50];
	char ip_addr[50];
	int port_no;
} Automata; 

extern Automata automata[4];

void automata_config(FILE *fpp);
size_t rest(size_t low, size_t high);

#endif