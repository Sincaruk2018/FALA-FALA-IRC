#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define DOMAIN "127.0.0.1"

struct sockaddr_in address;

int main() {
    int server_fd, set_socket_res, new_socket, valread;
    int addrlen = sizeof(struct sockaddr_in);
    char buffer[1024] = {0};

    // create server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {  // on error is returned -1
        printf("Houve  algum erro ao criar servidor\n");
        return 1;
    }

    // connect to a given port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)) < 0) {
        printf("Houve algum erro ao conectar a porta\n");
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // faz o bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("Houve algum erro ao realizar o bind\n");
        return 1;
    }

    // listen to a port to handle methods
    if (listen(server_fd, 3) < 0) {
        printf("Houve algum erro ao escutar porta\n");
        return 1;
    }
    // accept client
    if (new_socket = accept(server_fd, (struct sockaddr*)&address,
                            (socklen_t*)&addrlen) < 0) {
        printf("Houve algum erro ao aceitar cliente\n");
        return 1;
    }

    // le mensagem
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);

    // envia mensagem
    char* msgSend = "Mensagem enviada pelo server";
    send(new_socket, msgSend, sizeof(msgSend), 0);

    // encerra conexão com socket
    close(new_socket);

    // encerra escuta de sockets
    shutdown(server_fd, SHUT_RDWR);

    return 0;
}
