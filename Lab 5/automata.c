#include "automata.h"

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