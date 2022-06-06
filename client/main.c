#include <stdio.h>
#include <stdlib.h>
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

int main() {
	int aux_socket;
	int count_error = 0;
	int listening_state = 1;
	char message[4096];
	char recieved[4096];
	printf("Starting Client...");

	/* Try to get socket. Closes the program after successive failures*/
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
	printf("Got Socket sucessfully, you can start to chat now:\n\n\n\n\n\n\");

	/* Connect */

	while (1) {
		if (listening_state){ // Chat is ready to listen to message
			printf("Message: ");
			fgets(message, 4096, stdin); //Reads user input
			listening_state = 0; // No more messages, for now
		}

		/*Send*/ 
		printf("(Sending...)\n");
		send(message, strlen(message), 0);
		printf("(Sent)\n");
	}
	return 0;
}