#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define DOMAIN "127.0.0.1"


int createServerOnPort() {
    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    return server_fd;
}

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

    // faz o bind
    if (bind(server, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("Houve algum erro ao realizar o bind\n");
        return -1;
    }

    // listen to a port to handle methods
    if (listen(server, 3) < 0) {
        printf("Houve algum erro ao escutar porta\n");
        return -1;
    }

    if ((new_socket = accept(server, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return new_socket;
}

int sendMessage(int socket) {
    // envia mensagem
    char msg[4096];
    printf("Server: ");
    fgets(msg, 4096, stdin); //Reads user input
    return send(socket, msg, strlen(msg), 0);
}

int readMessage(int socket) {
    // lê mensagem
    int valread;
    char buffer[1024] = {0};

    valread = read(socket, buffer, 1024);
    printf("Client: %s\n", buffer);
    return valread;
}

void closeConnection(int socket) {
    // encerra conexão com socket
    close(socket);
}

void closeServer(int server) {
    // encerra escuta de sockets
    shutdown(server, SHUT_RDWR);
}

int main() {
    int server = createServerOnPort();
    if (server < 0) {
        printf("Houve  algum erro ao criar servidor\n");
        return 1;
    }

    int conn_socket = awaitConnectionOnPort(server, 8080);
    if (conn_socket < 0) {
        printf("Não foi possível se conectar a um cliente\n");
    } else {
        printf("Conexão realizada com sucesso\n");
    }

    int command = 0;

    while(1) {
        if(readMessage(conn_socket) < 0) {
            break;
        }

        if(sendMessage(conn_socket) < 0) {
            break;
        }
    }

    printf("Encerrando conexão\n");
    closeConnection(conn_socket);
    closeServer(server);

    return 0;
}
