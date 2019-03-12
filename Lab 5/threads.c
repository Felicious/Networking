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
Helper function for thread 2
establishes connection with other nodes

Pod 042 is suitable bc he is like the interactive
radio between the moon base's operator and 2B, and 
also gives her on-ground advice 
*/
void Pod_042(int *data, int machine){
    int sock;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    // configure address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(target->port);
    //take the ip address of the automata you're sending from
    inet_pton(AF_INET, automata[machine]->ip, &serverAddr.sin_addr.s_addr);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));  
    addr_size = sizeof serverAddr;

    // create udp socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("error setting up connection when sending [%d %d %d]",
            data[0], data[1], data[2]);
        return 0;
    }

    printf("sending new map [%d %d %d] to %d",
        data[0], data[1], data[2], automata[machine]->id);

    // message size is 3 * int
    if (sendto(sock, data, 3 * sizeof(int), 0,
        (struct sockaddr*) &serverAddr, addr_size) == -1) {
            printf("failed sending [%d %d %d]",
                data[0], data[1], data[2]);
            return 0;
    }
    
}

/* Another helper function for Thread 2
Function that sends new cost table (map) to neighbors
only. 

Is the Operator from Nier Automata because she shares
how much information is shared to 2B
*/

void Operator6O(Automata* automata, int* map){
    Automata machines[4];
    //id of the current machine we're on
   // ???? int current_id = automata[machine]->id; 
    for(int i = 0; i < 4; ++i){
        if(i == current_id){
            continue;
        }

        Pod_042(&machines[i], map);
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
        pod_042(map, machine);

		




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
