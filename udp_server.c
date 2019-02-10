
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

	int ack;
	int cksum;

	//open output file
	FILE *dest;
	
	while(1)
	{
		recvfrom (sock, received, 10, 0, (struct sockaddr *)&serverStorage, &addr_size); //receive file name from client
		perror("Received file from client\n");

		cksum = received->header.checksum;

		received->header.checksum = 0;
		received->header.checksum = calc_checksum(received, sizeof(HEADER) + received->header.length);
		
		int isreceivingfilename = 1;
		//if this is the output file 

		if(cksum != received->header.checksum)
		{  //checksum is incorrect
			printf("Received checksum : %d\n", cksum);
			printf("New checksum: %d\n", received->header.checksum);
			//change the ack to opposite
			ack = (received->header.seq_ack + 1) % 2; //change ack #
			received->header.seq_ack = ack;
		}
		else //if (cksum == received->header.checksum)
		{
			if((dest != NULL) && (received->header.length > 0) && (isreceivingfilename == 0))
			{ //received file is uncorrupted and we're ready to write into the file
				fwrite(received->data, 1, received->header.length, dest); //write into file
			}
			else if ((dest == NULL) && (received->header.length > 0) && (isreceivingfilename)) //haven't opened the file yet! 
			{ // just received uncorrupted output file name and make a new file called that!
				dest = fopen(received->data, "wb"); 
				if(!dest){ 
					printf("File cannot be opened\n");
					return 0;
				} //opened the file
			}
			else if((dest != NULL) && (received->header.length == 0) && (isreceivingfilename))
			{ //we just received the empty packet indicating that we're gonna start writing to the file now 
				isreceivingfilename = 0;
			}
			else if((dest != NULL) && (received->header.length == 0) && (isreceivingfilename == 0))
			{
				printf("Yay we finished reading ALL packets from the client :D\n");
				fclose(dest);
			}
		}
		//regardless, the server will send something back to the client
		sendto (sock, received, sizeof(received), 0, (struct sockaddr *)&serverStorage, addr_size);
	}
	return 0;
}
