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
	int sock, portNum, nBytes;
	char buffer[10];
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

	//create packets
	PACKET *outgoing = (PACKET * )malloc(sizeof(PACKET));
	PACKET *response = (PACKET * )malloc(sizeof(PACKET));

	/*Create UDP socket*/
    if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf ("socket error\n");
		return 1;
	}

	//send file name to server
	PACKET *sendpck = (PACKET *)malloc(sizeof(PACKET));

	outgoing->header.seq_ack = 0; //seq number is 0 at first (will only be 0 or 1)
	outgoing->header.length = sizeof(sendpck); //amt of bytes of data we have
	outgoing->header.checksum = 0;
	//outgoing->data = argv[3];
	memcpy(outgoing->data, argv[3], 10);

	//checking checksum with yourself
	outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);

	//now you're ready to send!
	sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);

	// receive
	recvfrom (sock, response, 10, 0, NULL, NULL);

	int resent = 0; //resent # of times counter
	//now you wanna check if you got the right message back
	while(outgoing->header.seq_ack != response->header.seq_ack){
		if (resent == 3){
			perror("Name of file was sent 3 times and failed ): \n");
			exit(-1);
		}else{
			//if the seq # don't match, resend
			perror("Name of file: The sequence # of outgoing and received packets don't match!\n");
			outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);
			sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);
		}
		resent++;
	}

	perror("Name of input file has been sent to server successfully\n");

	//send empty packet to server
	//once you're done sending the output
	outgoing->header.length = 0;
	memset(outgoing->data, '\0', sizeof(outgoing->data));
	outgoing->header.checksum = 0;
	outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);

	//now you're ready to send!
	sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);

	// receive
	recvfrom (sock, response, 10, 0, NULL, NULL);

	int resent = 0; //resent # of times counter
	//now you wanna check if you got the right message back
	while(outgoing->header.seq_ack != response->header.seq_ack){
		if (resent == 3){
			perror("Name of file was sent 3 times and failed ): \n");
			exit(-1);
		}else{
			//if the seq # don't match, resend
			perror("Sending the empty file: The sequence # of outgoing and received packets don't match!\n");
			outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);
			sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);
		}
		resent++;
	}


	//FINALLY READY TO SEND THE FILE THROUGH PACKETS
	//OH MY GOD
	FILE *src;
	src = fopen(argv[3], "rb");
	if(!src){
		printf("File cannot be opened\n");
		return 0;
	}

	int seq_num = 0;
	while (!feof(src))
	{
		//want to parse packet and prepare it before sending

		//open the file and store the message into the buffer
		//read 10 bytes at a time and put it into packet
		size_t sz = fread(outgoing->data, 1, 10, src);

		//initialize the packet header values
		outgoing->header.seq_ack = seq_num; //seq number is 0 at first (will only be 0 or 1)
		outgoing->header.length = sz; //amt of bytes of data we have
		outgoing->header.checksum = 0;

		//checking checksum with yourself
		outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);

		//now you're ready to send!
		sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);

		// receive
		recvfrom (sock, response, 10, 0, NULL, NULL);
			
		int resent = 0; //resent # of times counter
		
		//now you wanna check if you got the right message back
		while(outgoing->header.seq_ack != response->header.seq_ack){
			if (resent == 3){
				perror("Name of file was sent 3 times and failed ): \n");
				exit(-1);
			}else{
				//if the seq # don't match, resend
				perror("Name of file: The sequence # of outgoing and received packets don't match!\n");
				outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);
				sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);
			}
			resent++;
		}	


		//move onto next state
		seq_num = ((seq_num + 1) % 2);
	}

	fclose(src);

	//after reach the end of the input file, send empty packet to server so it knows when to close
	//send empty packet to server
	//once you're done sending the output
	outgoing->header.length = 0;
	memset(outgoing->data, '\0', sizeof(outgoing->data));
	outgoing->header.checksum = 0;
	outgoing->header.checksum = calc_checksum(outgoing, sizeof(HEADER) + outgoing->header.length);

	//now you're ready to send!
	sendto (sock, outgoing, sizeof(outgoing), 0, (struct sockaddr *)&serverAddr, addr_size);
	return 0;
}
