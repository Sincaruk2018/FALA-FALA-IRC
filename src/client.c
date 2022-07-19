#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

int setupAdress(struct sockaddr_in* addr, char* domain, int port) {
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  if (inet_pton(AF_INET, domain, &addr->sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  return 0;
}

int setupSocket() {
  int socket_fd;  // Or socket "id"
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  return socket_fd;
}

int Connect(int socket_fd, struct sockaddr_in* server_addr) {
  int connection_status =
      connect(socket_fd, (struct sockaddr*)server_addr, sizeof(*server_addr));
  if (connection_status < 0) {
    printf("\n Could not connect to Server \n");
    return -1;
  }

  return connection_status;
}

int main() {
  int mySocket = 0;
  struct sockaddr_in server_address;

  if (setupAdress(&server_address, DOMAIN, PORT) == -1) return 0;

  mySocket = setupSocket();
  if (mySocket == -1) return 0;

  char command[100] = "begin";

  while (strcmp(command, "quit") != 0) {
    printf(
        "Digite qual comando gostaria de executar:\n"
        "connect - conectar ao servidor\n"
        "quit - sair da aplicação\n"
        "ping - checar conexão com servidor\n"
        "Comando: ");
    scanf("%s", command);

    if (strcmp(command, "ping") == 0) {
      printf("\nping\n\n");
    } else if (strcmp(command, "connect") == 0) {
      printf(
          "\nConectando ao servidor, digite 'quit' para encerrar a conexão\n");

      if (Connect(mySocket, &server_address) == -1) {
        printf("Não foi possível realizar a conexão. Tente novamente\n");
      } else {
        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(server_address.sin_addr), buf, INET_ADDRSTRLEN);
        printf("Conectado em: %s:%hu\n", buf, ntohs(server_address.sin_port));
      }

      char usernameSend[] = {"Client"};
      char usernameRead[] = {"Server"};
      readAndSendMessages(mySocket, usernameSend, usernameRead);

      printf("Encerrando conexão\n\n");
    } else {
      printf("\nComando não reconhecindo tente novamente\n\n");
    }
  }

  // closing the connected socket
  close(mySocket);
  return 0;
}
