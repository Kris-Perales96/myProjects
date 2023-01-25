/* 
 * Echo client using TCP
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_TCP_PORT	3000
#define BUFLEN	256
#define TOTALEXECUTIONS 4

int main(int argc, char **argv)
{

 int n, bytes_to_read;
 int sd, port;
 struct hostent *hp;
 struct sockaddr_in server;
 char *host, *bp, rbuf[BUFLEN], sbuf[BUFLEN];

 /*
  * Parse command-line arguments
  */
 
 switch(argc) {
 case 2:
   host = argv[1];
   port = SERVER_TCP_PORT;
   break;
 case 3:
   host = argv[1];
   port = atoi(argv[2]);
   break;
 default:
   fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
   exit(1);
 }

 /* 
  * Create a stream socket 
  */

 if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
   fprintf(stderr, "ERROR: Can't create a socket\n");
   exit(1);
 }

 /* 
  * Bind an address to the socket 
  */

 bzero((char *)&server, sizeof(struct sockaddr_in));
 server.sin_family = AF_INET;
 server.sin_port = htons(port);
 if ((hp = gethostbyname(host)) == NULL) {
   fprintf(stderr, "ERROR: Can't get server address\n");
   exit(1);
 }
 bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

 /* 
  * Connect to server 
  */

 if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
   fprintf(stderr, "ERROR: Can't connect to server\n");
   exit(1);
 }

 printf("\nConnected: server address is %s\n", (char *)inet_ntoa(server.sin_addr));

 /* 
  * Transmit data to server
  */
  int inputData[BUFLEN-1];
  int executionCounter = 0;
	printf("Your options are add via a or A, subtraction via s or S\n");
	printf("multiplication via m or M, division via d or D\n");
	printf("Followed by at least two operands with a max of 8.\n");
	printf("Example would be a123/123 or a123z456 or m123 654 or d 144 -12\n\n");
while(1){
	
	printf("Getting data to send to server ...\n");
 	printf(".. enter some data: ");

 	memset(sbuf,0,BUFLEN);
 	fgets(sbuf,BUFLEN-1,stdin);
 	n = write(sd, sbuf, strlen(sbuf)-1);
 	if (n<0) {
   		fprintf(stderr, "ERROR: Problem writing to socket");
   		exit(1);
 	}

 /* 
  * Receive from server
  */

 	bzero(rbuf,BUFLEN);
 	n = read(sd, rbuf, BUFLEN-1);
 	if (n<0) {
   	fprintf(stderr, "ERROR: Problem reading from socket");
   	exit(1);
 	}
 	

 	printf(".. received [%s] (%d bytes) from server\n", rbuf,(int)strlen(rbuf));
 	for(int k =0; k < (int)strlen(rbuf);k++){
		inputData[k] = rbuf[k];
	}
	
	if((inputData[0] == 'b' || inputData[0] == 'B') && (inputData[1] == 'y' || inputData[1] == 'Y') && (inputData[2] == 'E' || inputData[2] == 'e')){
		printf("The server closed the connection.\n");
		break;
	}
	printf("\n");
	executionCounter++;
	
}
 

 /* 
  * Clean up and exit 
  */

 close(sd);
 return(0);
}
