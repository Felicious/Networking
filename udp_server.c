
/********************
 * COEN 146, UDP example, server
 ********************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

/********************
 * main
 ********************/
int main (int argc, char *argv[])
{
	int sock, nBytes;
	char buffer[10];
	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size, client_addr_size;
	int i;

    if (argc != 2)
    {
        printf ("need the port number\n");
        return 1;
    }

	// init 
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons ((short)atoi (argv[1]));
	serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	memset ((char *)serverAddr.sin_zero, '\0', sizeof (serverAddr.sin_zero));  
	addr_size = sizeof (serverStorage);

	//create packets
	PACKET *received = (PACKET * )malloc(sizeof(PACKET));

	// create socket
	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf ("socket error\n");
		return 1;
	}

	// bind
	if (bind (sock, (struct sockaddr *)&serverAddr, sizeof (serverAddr)) != 0)
	{
		printf ("bind error\n");
		return 1;
	}

	//open output file
	FILE *dest;
	recvfrom (sock, received, 10, 0, (struct sockaddr *)&serverStorage, &addr_size); //receive file name from client
	perror("Received file from client\n");

	//while we have incoming packets from client
	while(strlen(received->data) > 0){
		int cksum = received->header.checksum;

		received->header.checksum = 0;
		received->header.checksum = calc_checksum(received, sizeof(HEADER) + received->header.length);

		//if the checksums are not the same
		if(cksum != received->header.checksum){
			printf("Received checksum : %d\n", cksum);
			printf("New checksum: %d\n", received->header.checksum);
			//change the ack to opposite
			received->header.seq_ack = (received->header.seq_ack + 1) % 2;
		}else{ //if the checksums are the same, write to file 
			if(dest == NULL)
				dest = fopen(received->data, "wb"); //receive output file name
			if(!dest){
				printf("File cannot be opened\n");
				return 0;
			}
			fwrite(dest, received->header.length, received->data);
		}


		sendto (sock, received, sizeof(received), 0, (struct sockaddr *)&serverStorage, addr_size);

	}

	return 0;
}
