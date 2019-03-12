#include "threads.h"

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

void* A2(void* arg)
{
    int sock;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	//establish UDP server
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(automata[machine_id].port);			//set the right port from the machine table
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);					//don't care where its sent from 
	memset((char *)serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
	addr_size = sizeof(serverStorage);
	
    //creating a socket
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("socket error\n");
	}
    //bind
	if(bind(sock,(struct sockaddr *)&serverAddr, sizeof(serverAddr)) !=0){
		printf("bind error\n");
	}	

    int response[3];

    //listening loop!
    while(1)
    {
        printf("Waiting for response msg\n");

        recvfrom(sock, response, sizeof(response), 0,
            (struct sockaddr *)&serverStorage, &addr_size);
        printf("Received message: [%d %d %d]", response[0], response[1], response[2]);

        new_map_data();

    }
} 