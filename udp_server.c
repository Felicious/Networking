
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
	
	while(1){
		recvfrom (sock, received, 10, 0, (struct sockaddr *)&serverStorage, &addr_size); //receive file name from client
		perror("Received file from client\n");

		int cksum = received->header.checksum;
		int ack;

		received->header.checksum = 0;
		received->header.checksum = calc_checksum(received, sizeof(HEADER) + received->header.length);
		
		int isreceivingfilename = 1;
		//if this is the output file name
		if(cksum != received->header.checksum)
		{ //output file name and the checksum is incorrect
			printf("Received checksum : %d\n", cksum);
			printf("New checksum: %d\n", received->header.checksum);
			//change the ack to opposite
			ack = (received->header.seq_ack + 1) % 2; //change ack #
			received->header.seq_ack = ack;
		}else if((dest == NULL) && (cksum == received->header.checksum))
		{ //this is the uncorrupted dest file name
			dest = fopen(received->data, "wb"); //receive output file name
			if(!dest){
				printf("File cannot be opened\n");
				return 0;
			}
		}else if((received->header.length == 0) && (dest != NULL))
		{ //empty packet indicating starting to send file
			isreceivingfilename = 0;
		}else if((received->header.length == 0) && isreceivingfilename == 0)
		{ //reached the end of the packet and file
			fclose(dest);
			break;
		}else if((dest != NULL) && (cksum == received->header.checksum)){ //the checksum is correct, then u write
			fwrite(received->data, 1, received->header.length, dest);
		}
		sendto (sock, received, sizeof(received), 0, (struct sockaddr *)&serverStorage, addr_size);
	}

	

	return 0;
}
