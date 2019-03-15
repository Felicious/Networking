#include "automata.h"
#include <stdio.h>
#include <time.h>

Automata automata[4];

void automata_config(FILE *fpp){
    if(fpp == NULL){
		printf("Problem opening Automata naming file!\n");
		return;
	}

    for(int i = 0; i < 4; i++){
        //copying data from host name file 
        //into automata data struct
    	fscanf(fpp, "%s %s %d", automata[i].name, 
            automata[i].ip_addr, &automata[i].port_no);
    }

    fclose(fpp);

}

//function that sets random range for automata to sleep
//falls under one of the automata behaviors
size_t rest(size_t low, size_t high) {
    // persists between diff function operations
    static int init = 0;

    // call srand only first time
    if (!init) {
        srand(time(0)); 
        init = 1;
    }

    return rand() % (high + 1 - low) + low;
}