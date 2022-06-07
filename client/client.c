#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define DOMAIN "127.0.0.1"

int Setup_Adress(struct sockaddr_in* addr, char* domain, int port ){
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	if (inet_pton(AF_INET, domain, &addr->sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	return 0;
}

int Setup_Socket(){

	int socket_fd; // Or socket "id"
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}
	
	return socket_fd;
}

int Connect(int socket_fd, struct sockaddr_in* server_addr){
	int connection_status = connect(socket_fd, (struct sockaddr*)server_addr, sizeof(*server_addr));
	if (connection_status < 0)
	{
		printf("\n Could not connect to Server \n");
		return -1;
	}
	
	return connection_status;
}

int main()
{
	int mySocket = 0;
	struct sockaddr_in server_address;

	if(Setup_Adress(&server_address, DOMAIN, PORT) == -1) return 0;

	mySocket = Setup_Socket();
	if (mySocket == -1) return 0;
	
	if(Connect(mySocket, &server_address) == -1) return 0;
	else{
		char buf[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(server_address.sin_addr), buf, INET_ADDRSTRLEN);
		printf("Connection Successful!\nConnected at: %s:%hu\n", buf, ntohs(server_address.sin_port));
	}

	char message[4096];
	char recieved[4096];
	int nbytes;
	while(1) {    // get message from stdin
		printf("Client: ");
		fgets(message, 4096, stdin);

		if(send(mySocket, message, strlen(message), 0) < 0) {   // write message to server
			printf("Error on send\n");
			exit(1);
		}					
		if((nbytes=read(mySocket,recieved, 100))==-1) {     // read message from server
			printf("Error read\n");
			exit(1);
		}
		recieved[nbytes]='\0';
		printf("Server: %s\n", recieved);
   }

	// closing the connected socket
	close(mySocket);
	return 0;
}
