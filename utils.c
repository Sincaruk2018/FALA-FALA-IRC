#include "utils.h"
#include "commands.h"

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
int readAndSendMessages(int socket, char* usernameSend, int* isMuted) {
  int commandID = -1;
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
        /*if (strcmp(buf, quitCommand) == 0) {
          printf("Desconectando\n");
          return 0;
        }*/

        /* EN: Changed the logic. Instead of using strcmp on the main function and here, we got
        a new module to deal with commands. Instead of doing lots of stcmp, we can compare only
        the first char for commands, which improves efficiency

        PT-BR: Mudei a lógica. AO invés de usar strcmp na função main e aqui, nós temos um módulo
        novo para lidar com comandos. Ao invés de fazer vários strcmp, nós podemos comparar apenas
        o primeiro char para comandos, o que melhora a eficiência.
        
        */
        if(buf[0] == '/'){
          commandID = ExecuteCommand(buf);
          if (commandID == 0) return END_PROGRAM;
        }
        
        // Send message to client
        char str[4096];
        strcpy(str, usernameSend);
        strcat(str, ": ");
        strcat(str, buf);

        if (send(socket, str, strlen(str) + 1, 0) < 0) {
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
      } else {
        // read message
        printf("%s\n", reply);
      }

      memset(reply, 0, sizeof(reply));
    }
  }

  return 0;
}

int readAndSendMessagesServer(int serverSocket, int maxClients,
                              int* clientSocket) {
  int rc;
  fd_set all_set, r_set;

  r_set = all_set;
  char buf[BUF_SIZE], reply[BUF_SIZE];

  char quitCommand[] = "quit\n";

  int newSocket;

  struct sockaddr_in address;
  int addrlen = sizeof(address);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  while (1) {
    FD_ZERO(&all_set);
    FD_SET(serverSocket, &all_set);
    int max_sd = serverSocket;

    // adiciona no SET os sockets válidos
    for (int i = 0; i < maxClients; i++) {
      int sd = clientSocket[i];

      if (sd > 0) FD_SET(sd, &all_set);

      if (sd > max_sd) max_sd = sd;
    }

    rc = select(max_sd + 1, &all_set, NULL, NULL, NULL);

    if (rc <= 0) {
      return -1;
    }

    if (FD_ISSET(serverSocket, &all_set)) {
      // Conecta novo cliente
      if ((newSocket = accept(serverSocket, (struct sockaddr*)&address,
                              (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
      } else {
        printf("Novo cliente conectado\n");
      }

      // Adiciona novo socket no array
      for (int i = 0; i < maxClients; i++) {
        if (clientSocket[i] == 0) {
          clientSocket[i] = newSocket;
          printf("Socket adicionado na lista na posição %d\n", i);

          break;
        }
      }
    }

    for (int i = 0; i < maxClients; i++) {
      int sd = clientSocket[i];

      if (FD_ISSET(sd, &all_set)) {
        // Tenta ler input do socket
        if ((rc = recv(sd, reply, BUF_SIZE, 0)) == 0) {
          // Cliente desconectou, fecha conexão
          printf("Cliente desconectou\n");

          close(sd);
          clientSocket[i] = 0;
        } else {
          // Exibe mensagem do cliente
          printf("%s\n", reply);

          // envia mensagem para todos os outros clientes
          for (int j = 0; j < maxClients; j++) {
            if (clientSocket[j] == 0 || i == j) {
              continue;
            }

            send(clientSocket[j], reply, strlen(reply), 0);
          }
        }

        memset(reply, 0, sizeof(reply));
      }
    }
  }

  return 0;
}