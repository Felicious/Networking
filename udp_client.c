/*****************************
 * COEN 146, UDP, client
 *****************************/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
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

	if (argc != 5)
	{
		printf ("need the port number and machine\n");
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
	
	while(1){

		//initializing packet
		outgoing->header.seq_ack = seq_num; //seq number is 0 at first (will only be 0 or 1)
		ack = outgoing->header.seq_ack;
		outgoing->header.checksum = 0; //later initialze checksum to random number

		//checking checksum with yourself
		outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);
		cksum = outgoing->header.checksum;

		
		if((read_file_name)&&(sending_empty_packet == 0)) //if you're trying to read the file name
		{ 
			//copy the 4th parameter(which contains the output file name)
			memcpy(outgoing->data, argv[4], 10);
			outgoing->header.length = strlen(argv[4]);
		}			
		else if((sending_empty_packet == 0) && read_file_name == 0)  //save content of file
		{
			//open the file and store the message into the buffer
			outgoing->header.length = fread(outgoing->data, 1, 10, src); //store stuff directly into packet data
		}
		else //if(sending_empty_packet == 1) 
		//need to send an empty packet after finish sending the output file name
		{
			outgoing->header.length = 0;
		}
		
		//if the size is 0, means we reached the end of the msg or we wanna send an empty packet (flag)
		//we're gonna send an empty packet
		if(outgoing->header.length == 0)
		{
			memset(outgoing->data, '\0', sizeof(outgoing->data));
		}

		//DONE INITIALIZING PACKET HEADER VALUES!
		perror("preparing to send packet\n");
		sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);

		// receive
		recvfrom (sock, outgoing, 10, 0, NULL, NULL);

		if(resent == 3){
			printf("Packet was resent 3 times and failed ):\n");
			if(read_file_name == 1)
				printf("The offending file is the output file name ):<\n");
			else
				printf("The offending file is the msg O:<\n");
			break;
		}
		
		if(ack != outgoing->header.seq_ack)
		{ //seq #'s don't match (for both filename and packet contents)
			//resend the file
			resent++;
			printf("the acknowledgement numbers dont match /: \n");
			seq_num = ((outgoing->header.seq_ack + 1) % 2);
			continue;
		}

		//if the code reached here, that means the ack #'s match. 
		//it would have continued to next loop or broke if it didnt

		//a packet was sent successfully with no corruptions + mismatched ack #!
		if(cksum == outgoing->header.checksum){
			resent = 0;
			
			//if these conditions apply, then the msg from packet was sent successfully
			if((read_file_name == 0) && (sending_empty_packet == 0))
			{
				//yay the msg can be sent!
			}
			else if((read_file_name == 1)&&(sending_empty_packet == 0))
			{ //means we just successfully sent the outputfilename
				
				//next, we want to send an empty packet so flag it
				sending_empty_packet = 1;
			} //also would never find youself in a (0 1) situation bc that means you're sending an empty packet when you're reading from msg, which will never happen bc the send empty packet flag wont go up when reading msg  
			else if((read_file_name == 1)&&(sending_empty_packet == 1))
			{
				//currently sending the empty packet to exit out of reading the output name
				read_file_name = 0;

				//next we wanna make sure we're NOT sending an empty packet and start sending the msg 
				sending_empty_packet = 0;
			}
			else if((read_file_name == 0) && (outgoing->header.length == 0))
			{ 
				printf("WE'RE FINALLY DONE HOLY SHIT!!!!!\n");
				break;
			}
		}
		else //else: there's corruption. should send again ):
		{
			printf("checksums dont match! Outgoing: %d", ", versus Response: %d", chksum, outgoing->header.checksum);
			resent++;
		}

		seq_num = ((outgoing->header.seq_ack + 1) % 2);

	} //END OF WHILE LOOP (FINALLY) 

	fclose(src);
	
	return 0;
}
