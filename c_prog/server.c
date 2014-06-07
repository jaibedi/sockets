#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFSIZE 1024

void error(const char*);

int main()
{
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
	printf("Echo Server Listening....\n");	
	while (1) {
		char buffer[BUFFSIZE];
		unsigned int address_size = sizeof(client_addr);  // size of structure holding client sddress
	
		memset(buffer, 0, BUFFSIZE);  // initialize buffer to 0

		// accept incomming connections
		int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
		if (connect_d == -1)
			error("Can't open secondary socket");
			
		int len = 0;
		while ((len = recv(connect_d, buffer, BUFFSIZE, 0))) {
			
			printf("%s", "from client: ");
  		fputs(buffer, stdout);

			send(connect_d, buffer, len, 0);  //send data back to client
			
			memset(buffer, 0, BUFFSIZE);  // flush buffer to 0
		}
	
		close(connect_d);
	}
	
	return 0;
}

void error(const char *msg)
{
	printf("%s", msg);
	exit(1);
}
