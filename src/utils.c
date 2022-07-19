#include "utils.h"

#include <fcntl.h>  // used to be able to get input non blocking
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* Read and send messages in a assyncronous way
select is used to detect whether the connection has something to
receive or if the get input is ready to be send, after is detected
we can execute the blocking methods (fgets, recv, send)
*/
int readAndSendMessages(int socket, char* usernameSend, char* usernameRead) {
  int rc;
  fd_set all_set, r_set;

  FD_ZERO(&all_set);
  FD_SET(STDIN_FILENO, &all_set);
  FD_SET(socket, &all_set);
  r_set = all_set;
  char buf[BUF_SIZE], reply[BUF_SIZE];

  char quitCommand[] = "quit\n";

  while (1) {
    r_set = all_set;

    rc = select(socket + 1, &r_set, NULL, NULL, NULL);

    if (rc <= 0) {
      return -1;
    }

    if (FD_ISSET(STDIN_FILENO, &r_set)) {
      if (fgets(buf, BUF_SIZE, stdin)) {
        // command  quit
        if (strcmp(buf, quitCommand) == 0) {
          printf("digitou quit\n");
          return 0;
        }

        // Send message to client
        if (send(socket, buf, strlen(buf) + 1, 0) < 0) {
          puts("Send failed");
          return -1;
        }
      }
    }

    if (FD_ISSET(socket, &r_set)) {
      // Get message from client
      if ((rc = recv(socket, reply, BUF_SIZE, 0)) < 0) {
        puts("recv failed");
        return -1;
      }

      if (strcmp(reply, "ping\n") == 0) {
        // Send pong
        send(socket, "pong\0", 5, 0);
        printf("\n%s: ping\n", usernameRead);
        printf("PONG\n");
      } else {
        // read message
        printf("\n%s: %s\n", usernameRead, reply);
      }

      reply[0] = '\0';
    }
  }

  return 0;
}