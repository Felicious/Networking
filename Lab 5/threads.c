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

        pthread_mutex_lock(&lock);
        recvfrom(sock, response, sizeof(response), 0,
            (struct sockaddr *)&serverStorage, &addr_size);
        printf("Received message: [%d %d %d]", response[0], response[1], response[2]);

        //send response to function that updates 
        //cost matrix information
        new_map_data(response);

        //unlock after finished 
        pthread_mutex_unlock(&lock);
        printf("Received and updated matrix: \n");
        pmatrix(map_database);
    }
} 

/*
THREAD # 2
Sender thread

Called 2B because she perpetuates the story in Nier
Automata, even following her death ):

*/
void TwoB(){
    //not sure how many times to loop
    //depends on total # of automata - 2
    for(int i = 0; i < 2; i++){
        sleep(10);
        int machine; //target: 2B's mission to kill
        int mission; //mission: new cost

        //get user input
        printf("Enter <machine id> <new_cost>: ");
        fflush(stdout);

        scanf("%d %d", &machine, &mission);

        if(machine > 3 || machine < 0) {
            printf("Wrong machine id\n");
            continue;
        }

        printf("sending new map data from automata %d to %d with new cost %d\n",
            machine_id, machine, mission);

        //creates map and puts it in the format neccesary
        int map[3];
        map[0] = machine_id;
        map[1] = machine; //from
        map[2] = mission; //to

        //update map
        new_map_data(map);

        //send new map
        //make new function?
        
			//establish udp connection with the host table to get the ip and socket
			if(machine_id != i){
				//configure ip address
				serverAddr.sin_family = AF_INET;
				serverAddr.sin_port = htons(linux_machines[i].port);				//destination port number
				inet_pton(AF_INET, linux_machines[i].ip, &serverAddr.sin_addr.s_addr);		//destination ip addresss
				memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
				addr_size = sizeof serverAddr;
				//Create UDP Socket
				if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
					printf("socket error\n");
					return 1;
				}
				sendto(sock,cost_update, sizeof(cost_update), 0, (struct sockaddr *)&serverAddr, addr_size);	//send to destination, won't wait for confimation
			}
		}




    }
}

/*
THREAD # 3
Updater thread

Repeats forever, following random delays, runs the
algo to update the least path

It's called NineS because 9S supports 2B in Nier, &
the updater thread updates and maintains the matrix
to provide useful info for the algorithm's use.
*/
void* NineS(void* arg){
    while(1){
        //let my cute bby 9S sleep (:
        size_t clk = rest(10, 20);
        printf("Sleeping for %d seconds", clk);
        sleep(clk);

        printf("Current map: \n");
        pmatrix(map_database);

        int* new_map = nier(map_database, machine_id);
        printf("Least cost array (closest path):\n");
        pmatrix(map_database);

        free(new_map); //empty for next time
    }
}
