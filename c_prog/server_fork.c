#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFSIZE 1024

void (*handlechild)(int);
void echoServer(int);
void timeServer(int);
void error(const char*);

int main(int argc, char *argv[])
{
	/* Select Server
	 * 1. EchoServer
	 * 2. TimeServer
	*/

	switch (atoi(argv[1])) {
		case 1:
				handlechild = echoServer;
				break;
		case 2:
				handlechild = timeServer;
				break;
		default:
				printf("\n Not a valid argument");
				exit(0);
	}	
	
	struct sockaddr_in server_addr, client_addr;
	int listener_d = 0;
	
	//creation of the socket
  listener_d = socket (AF_INET, SOCK_STREAM, 0);
	
	//preparation of the socket address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = (in_port_t)htons(2500);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// Set socket for reuse
	int reuse = 1;	
	if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
		error("Can't set the reuse option on the socket");
		
	// Bind the socket
	int c = bind (listener_d, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (c == -1)
		error("Can't bind to socket");
		
	// Listen
	if (listen(listener_d, 2) == -1)
		error("Can't listen");
	printf("Server Listening....\n");	
	while (1) {
		
		// accept incomming connections
		unsigned int address_size = sizeof(client_addr);  // size of structure holding client sddress
		int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
		if (connect_d == -1)
			error("Can't open secondary socket");
			
		/* Optional only to print IP address and Port of client */
		char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
      printf("Handling client %s/%d\n", clntName, ntohs(client_addr.sin_port));
    else
      puts("Unable to get client address");      
    /* End of Optional */     
      
		int childpid = fork();
		if (childpid == 0) {
			// Inside child
			close(listener_d);  // inside child you dont require listener_d
			handlechild(connect_d);
		}		
		else 
			close(connect_d);

	}	// End of while(1)
	
	return 0;
}

void echoServer(int connection)
{
	char buffer[BUFFSIZE];
	memset(buffer, 0, BUFFSIZE);  // initialize buffer to 0
	int len = 0;
	while ((len = recv(connection, buffer, BUFFSIZE, 0))) {
		
		printf("%s", "from client: ");
		fputs(buffer, stdout);

		send(connection, buffer, len, 0);  //send data back to client
		
		memset(buffer, 0, BUFFSIZE);  // flush buffer to 0
	}
	close(connection);
	exit(0);
}

void timeServer(int connection)
{
	time_t rawtime;
  struct tm *info;

  char buffer[BUFFSIZE];
	memset(buffer, 0, BUFFSIZE);  // initialize buffer to 0
	
  time(&rawtime);
  info = localtime(&rawtime);

  strftime(buffer, BUFFSIZE, "%d-%m-%Y %H:%M:%S", info);

	int result = send(connection, buffer, strlen(buffer), 0);
	if (result == -1) 
		error("Error talking to the client");
		
	close(connection);
	exit(0);		
}

void error(const char *msg)
{
	printf("%s", msg);
	exit(1);
}
