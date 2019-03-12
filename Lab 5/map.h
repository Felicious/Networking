#ifndef MAP_H
#define MAP_H

#include <pthread.h>
#include <stdio.h>

extern map_database[4][4];
extern host_no;

//function that parses the cost matrix from file
//returns cost table 
void init_matrix(FILE *cost);

//function that prints the matrix
void pmatrix(int** Map);

//update matrix (map function)
void new_map_data(int** Map, int* data);

#endif