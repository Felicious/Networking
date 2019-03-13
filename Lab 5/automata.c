#include "automata.h"

Automata automata[4];

void automata_config(FILE *fpp){
    if(fpp == NULL){
		printf("Problem opening Automata naming file!\n");
		return 0;
	}

    for(int i = 0; i < host_no; i++){
        //copying data from host name file 
        //into automata data struct
    	fscanf(fpp, "%s\t%s\t%d\n", automata[i].name, 
            automata[i].ip, &automata[i].port);
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