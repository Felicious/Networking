#ifndef NIER_H
#define NIER_H

#include <pthread.h>
#include <stdio.h>

int find_shortest(int *paths, int &visited);
int* nier(int **Map, int access_pt);