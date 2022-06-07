#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "minorutil.h"

/* Later modularization */

/* 
* void Setup_Socket();
* 
* void Connect();
* 
* void Send();
*/

/* Auxiliar function for address searching*/
/*char* New_Adress(int num)
{
	char num_str[4];
	sprintf(num_str, "%d", num);
	char* aux = "127.0.0.";
	char* return_str = (char*) malloc( *sizeof(char));
	return return_str;
}*/

int main() {
	FILE* verbose;
	struct sockaddr_in socket_address;
	int aux_socket, inet_pton_status, client_status, servalue, listening_server_state;
	int count_error = 0;
	int listening_state = 1;
	int listening_outside = 1;
	char message[4096];
	char recieved[4096];
	printf("Starting Client...");

	verbose = fopen("report.txt","w+");

	/* Trying to get socket. Closes the program after successive failures*/
	aux_socket = socket(AF_INET, SOCK_STREAM, 0); // Command line to get socket
	while (aux_socket < 0) {
		printf("Problem in getting socket. Trying again...");
		count_error++;
		if (count_error > MAX_ERROR) {
			printf("Couldn't get socket");
			return -1;
		}
		aux_socket = socket(AF_INET, SOCK_STREAM, 0); // Try again
	}

	printf("Got Socket sucessfully, you can start to chat now:\n\n\n\n\n\n");
	fprintf(verbose,"Socket number: %d\n", aux_socket);

	/* Connect. Refine this */
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(8080);
	inet_pton_status = inet_pton(AF_INET,"127.0.0.1", &socket_address.sin_addr);

	if (inet_pton_status <= 0){
		printf("Invalid address, try another.");
		fprintf(verbose,"Couldn't use address 127.0.0.69\ninet status:%d\n",inet_pton_status);
		return -1;
	}

	/* Try to find the first server's address -->  IMPLEMENT LATER, NOT FOR FIRST PART*/
	/*count_error = 0;
	while (inet_pton_status <= 0)
	{
		count_error++;
		inet_pton_status = inet_pton(AF_INET, New_Adress(count_error+1), &socket_address.sin_addr);
		if (count_error >= MAX_ERROR){
			printf("Couldn1t find a valid address");
		}
	}*/

	client_status = connect(aux_socket, (struct sockaddr*)&socket_address, sizeof(socket_address));
	if (client_status < 0) {
		printf("Couldn't connect client\n");
		fprintf(verbose, "Client FAILED\n");
		return -1;
	}

	servalue = 0;
	listening_server_state = 1;
	listening_state = 0;
	
	int nbytes;
	while(1) {    // get message from stdin
		printf("Client: ");
		fgets(message, 4096, stdin);

		if(send(aux_socket, message, strlen(message), 0) < 0) {   // write message to server
			printf("Error on send\n");
			exit(1);
		}					
		if((nbytes=read(aux_socket,recieved, 100))==-1) {     // read message from server
			printf("Error read\n");
			exit(1);
		}
		recieved[nbytes]='\0';
		printf("Server: %s\n", recieved);
   }

	printf("Encerrando conexão\n");

	fclose(verbose);
	return 0;
}