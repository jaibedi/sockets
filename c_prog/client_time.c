#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFSIZE 1024

void error(const char*);

int main()
{
	struct sockaddr_in sock_addr;
	int sock_d = 0;
	char recvbuffer[BUFFSIZE];
	
	//Create a socket for the client
  //If sock_d < 0 there was an error in the creation of the socket
  if ((sock_d = socket (AF_INET, SOCK_STREAM, 0)) < 0)
  	error("Problem in creating the socket");
	
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sock_addr.sin_port = htons(2500);
	
	//Connect to the server
  if (connect(sock_d, (struct sockaddr *) &sock_addr, sizeof(sock_addr))<0) 
	  error("Problem in connecting to the server");	
		
	memset(recvbuffer, 0, BUFFSIZE);  // flush buffer to 0		
	if (recv(sock_d, recvbuffer, BUFFSIZE,0) == 0)
	  //error: server terminated prematurely
		error("The server terminated prematurely"); 
		
	printf("%s", "Received from server: ");
	fputs(recvbuffer, stdout);
	
	close(sock_d);	
	
	return 0;
}

void error(const char *msg)
{
	printf("%s", msg);
	exit(1);
}
