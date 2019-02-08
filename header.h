

#define SIZE 10
typedef struct{
	int seq_ack; //will always be 0 or 1
	int length; //how many bytes of data u have
	int checksum; //checksum calc by XOR bytes
}HEADER;

typedef struct{
	HEADER header;
	char data[SIZE];
}PACKET;

int calc_checksum(PACKET *pkt, int nBytes){
	int i = 0;
	char cs = 0;
	char *p = (char*)pkt;
	for (int i = 0; i < nBytes; ++i){
		cs = cs ^ *p;
		++p;
	}
	return(int)cs;
}
