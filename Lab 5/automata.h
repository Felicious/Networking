#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Automata automata[4];
extern host_no;

void automata_config(FILE *fpp);
size_t rest(size_t low, size_t high);

