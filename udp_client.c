/*****************************
 * COEN 146, UDP, client
 *****************************/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "header.h"

/***********
 *  main
 ***********/
int main (int argc, char *argv[])
{
	perror("this is the top of server\n");
	int sock, portNum, nBytes;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	if (argc < 5)
	{
		printf ("parameters: %s <port> <ip> <input> <output> [rand]\n");
		return 1;
	}

	// configure address
	//when sending packet, 1st parameter is port #
	// 2nd is IP address
	// 3rd is source file name
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons (atoi (argv[1]));
	inet_pton (AF_INET, argv[2], &serverAddr.sin_addr.s_addr);
	memset (serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero));  
	addr_size = sizeof serverAddr;

	/*Create UDP socket*/
    if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf ("socket error\n");
		return 1;
	}
	
	FILE *src; //will be used to open the source file at first
	src = fopen(argv[3], "rb");
	if(!src){
		printf("File cannot be opened\n");
		return 0;
	}

	PACKET *outgoing = (PACKET *)malloc(sizeof(PACKET)); //all outgoing data will be sent here
	int resent = 0;
	int seq_num = 0;
	
	//stores the seq no. and checksum of the outgoing file 
	//later, outgoing will indicate the response
	int ack;
	int cksum;
	//flags
	int read_file_name = 1; //flag indicating whether we're reading packet data (src) or output file name(filename) file
	int sending_empty_packet = 0; //flag indicating whether we're sending an empty packet (initialized to no)

	//set up timer stuff
	//borrowed from TA
	// local variablesneeded
	struct timeval tv; //timer
	int rv; // select returned value

	// set it up, in the beginning of the function
	fd_set readfds;
	fcntl(sock, F_SETFL, O_NONBLOCK);

	//seed random function
	 srand(time(0));
	
	while(1){

		//initializing packet
		if((read_file_name)&&(sending_empty_packet == 0)) //if you're trying to read the file name
		{ 
			//copy the 4th parameter(which contains the output file name)
			memcpy(outgoing->data, argv[4], 10);
			outgoing->header.length = strlen(argv[4]);
			printf("Sending file name:  %s\n", outgoing->data);
		}			
		else if((sending_empty_packet == 0) && read_file_name == 0)  //save content of file
		{
			//open the file and store the message into the buffer
			outgoing->header.length = fread(outgoing->data, 1, sizeof(outgoing->data), src); //store stuff directly into packet data
			printf("sending the message to svr:%s \n", outgoing->data);
		}
		else //if(sending_empty_packet == 1) 
		//need to send an empty packet after finish sending the output file name
		{
			outgoing->header.length = 0;
		}
		
		outgoing->header.seq_ack = seq_num; //seq number is 0 at first (will only be 0 or 1)
		outgoing->header.seq_ack;
		outgoing->header.checksum = 0; //later initialze checksum to random number

		//checking checksum with yourself
		outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);
		cksum = outgoing->header.checksum;

		
		
		//if the size is 0, means we reached the end of the msg or we wanna send an empty packet (flag)
		//we're gonna send an empty packet
		if(outgoing->header.length == 0)
		{
			memset(outgoing->data, '\0', sizeof(outgoing->data));
			resent++;
		}

		//implementing random checksum
		if(argc == 6)
		{
			//set checksum to 0 5% of the time
			if(rand() % 100 < 5){
				outgoing->header.checksum = 0;
			}
			else{
				outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);
			} 
		}

		//DONE INITIALIZING PACKET HEADER VALUES!
		perror("preparing to send packet\n");
		

		do
		{ //seq #'s don't match (for both filename and packet contents)
			//resend the file
			

			//initialize the timer
			FD_ZERO(&readfds); 
			FD_SET(sock, &readfds);
			tv.tv_sec = 1; //how long the timer is: wait 1 second
			tv.tv_usec = 0; //microseconds

			sendto (sock, outgoing, sizeof(*outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);

			//if theres no response from client in 1 second, resend
			//time we waited for the server to respond back to us
			// call select
			
			rv = select (sock + 1, &readfds, NULL, NULL, &tv);// sock is the socket you are using
			if (rv == 0)
			{// timeout, no data so resend
				if(resent == 3)
				{
					printf("resent the timed out file 3 times and failed\n");
					break;
				}
				perror("timed out, resending packet!\n");
				resent++;
				continue;
			}

			// receive
			recvfrom (sock, outgoing, sizeof(*outgoing), 0, NULL, NULL);

			if(resent == 3)
			{
				printf("Packet was resent 3 times and failed ):\n");
				if(read_file_name == 1)
					printf("The offending file is the output file name ):<\n");
				else
					printf("The offending file is the msg O:<\n");
				break;
			}

			if(seq_num != outgoing->header.seq_ack){
				printf("the acknowledgement numbers dont match /: \n");
			}

			if((outgoing->header.length == 0) && (seq_num != outgoing->header.seq_ack)){
				resent++;
			}

		}while(seq_num != outgoing->header.seq_ack);

		
		//if the code reached here, that means the ack #'s match. 
		//it would have continued to next loop or broke if it didnt

		//a packet was sent successfully with no corruptions + mismatched ack #!
		resent = 0;
			
		//if these conditions apply, then the msg from packet was sent successfully
		if((read_file_name == 0) && (sending_empty_packet == 0)&&(outgoing->header.length > 0))
		{
			printf("Yay~ sending msg of size: %d\n", outgoing->header.length);
	
		}
		else if((read_file_name == 1)&&(sending_empty_packet == 0))
		{ //means we just successfully sent the outputfilename
		
			//next, we want to send an empty packet so flag it
			read_file_name = 0;
		} //also would never find youself in a (0 1) situation bc that means you're sending an empty packet when you're reading from msg, which will never happen bc the send empty packet flag wont go up when reading msg  
		else if((read_file_name == 0) && (outgoing->header.length == 0))
		{ 
			printf("WE'RE FINALLY DONE HOLY SHIT!!!!!\n");
			sending_empty_packet = 1;
			break;
		}

		seq_num = ((outgoing->header.seq_ack + 1) % 2);

	} //END OF WHILE LOOP (FINALLY) 

	fclose(src);
	
	return 0;
}
