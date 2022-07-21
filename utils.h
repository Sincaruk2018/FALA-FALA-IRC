#ifndef UTIL_H
#define UTIL_H
#endif

#ifndef MAX_ERROR
#define MAX_ERROR 256
#endif

#ifndef PORT
#define PORT 8082
#endif

#ifndef BUF_SIZE
#define BUF_SIZE 4096
#endif

#ifndef DOMAIN
#define DOMAIN "127.0.0.1"
#endif

#ifndef END_PROGRAM
#define END_PROGRAM 363
#endif

int readAndSendMessages(int socket, char* usernameSend);
int readAndSendMessagesServer(int serverSocket, int maxClients,
                              int* clientSocket);