
/********************
 * COEN 146, UDP example, server
 ********************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

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
	PACKET *give_to_client = (PACKET * )malloc(sizeof(PACKET));

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
	dest = fopen("output.txt", "wb");
	if(!dest){
		printf("File cannot be opened\n");
		return 0;
	}

	while (1)
	{
		// receive  datagrams
		recvfrom (sock, received, 10, 0, (struct sockaddr *)&serverStorage, &addr_size);
		perror("Received file from client\n");
		

		// convert message
		for (i = 0; i < nBytes - 1; i++)
			buffer[i] = toupper (buffer[i]);

		// send message back
		sendto (sock, buffer, nBytes, 0, (struct sockaddr *)&serverStorage, addr_size);
	}

	return 0;
}
