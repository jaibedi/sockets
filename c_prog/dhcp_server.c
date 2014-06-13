/* Run with client program client_time.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFSIZE 1024
#define MAXADDR 5

void (*handlechild)(int);
void dnsServer(int);
void error(const char*);
void initialize();

struct IP {
	char *address;
	int flag;
};

struct IP address_table[5];

int main(int argc, char *argv[])
{
	initialize();

	handlechild = dnsServer;
	
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
      
		handlechild(connect_d);		
		close(connect_d);

	}	// End of while(1)
	
	return 0;
}



void dnsServer(int connection)
{
	int i = 0;	
	for ( i = 0; i < MAXADDR; i++) {
		if (address_table[i].flag == 0) {
				
			int result = send(connection, address_table[i].address, strlen(address_table[i].address), 0);
			if (result == -1) 
				error("Error talking to the client");							

			address_table[i].flag = 1;   // set the flag to 1, which indicates that the address has already been allocated
			break; // come out of for loop
		}
	}		
}

void initialize() {
	/* Initialize the IP struct holding IP addresses
	 * to be assigned to the clients
	*/
	
	address_table[0].address = "192.168.1.2";
	address_table[0].flag = 0;
	address_table[1].address = "192.168.1.3";
	address_table[1].flag = 0;
	address_table[2].address = "192.168.1.4";
	address_table[2].flag = 0;
	address_table[3].address = "192.168.1.5";
	address_table[3].flag = 0;
	address_table[4].address = "192.168.1.6";
	address_table[4].flag = 0;
}

void error(const char *msg)
{
	printf("%s", msg);
	exit(1);
}
