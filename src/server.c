#include <fcntl.h>  // used to be able to get input non blocking
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

/* Create server socket  */
int createServer() {
  int server_fd;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  return server_fd;
}

/* Listen to a given port and if a client try to reach it establish the
 * connection
 */
int awaitConnectionOnPort(int server, int port) {
  int opt = 1;
  int new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  // connect to a given port
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) < 0) {
    printf("Houve algum erro ao conectar a porta\n");
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server, (struct sockaddr*)&address, sizeof(address)) < 0) {
    printf("Houve algum erro ao realizar o bind\n");
    return -1;
  }

  // listen to a port to await a client
  if (listen(server, 3) < 0) {
    printf("Houve algum erro ao escutar porta\n");
    return -1;
  }

  // connect client
  if ((new_socket = accept(server, (struct sockaddr*)&address,
                           (socklen_t*)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  return new_socket;
}

/* Send a message to the client */
int sendMessage(int socket) {
  char msg[4096];
  printf("Server: ");
  fgets(msg, 4096, stdin);  // Reads user input
  return send(socket, msg, strlen(msg), 0);
}

/* Read client message */
int readMessage(int socket) {
  int valread;
  char buffer[1024] = {0};

  valread = read(socket, buffer, 1024);
  printf("Client: %s\n", buffer);
  return valread;
}

/* Close connection with client */
void closeConnection(int socket) { close(socket); }

/* Close server */
void closeServer(int server) { shutdown(server, SHUT_RDWR); }

int main() {
  int server = createServer();
  if (server < 0) {
    printf("Houve  algum erro ao criar servidor\n");
    return 1;
  }

  int conn_socket = awaitConnectionOnPort(server, PORT);
  if (conn_socket < 0) {
    printf("Não foi possível se conectar a um cliente\n");
  } else {
    printf("Conexão realizada com sucesso\n");
  }

  char usernameSend[] = {"Server"};
  char usernameRead[] = {"Client"};
  readAndSendMessages(conn_socket, usernameSend, usernameRead);

  printf("Encerrando conexão\n");
  closeConnection(conn_socket);
  closeServer(server);

  return 0;
}
