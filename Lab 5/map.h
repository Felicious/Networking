#ifndef MAP_H
#define MAP_H

#include <pthread.h>
#include <stdio.h>


//function that parses the cost matrix from file
//returns cost table 
int** init_matrix(FILE *cost);

//function that prints the matrix
void pmatrix(int** Map);

//update matrix (map function)
void new_map_data(int** Map, int* data);

#endif