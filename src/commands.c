#include "commands.h"
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

void PrintComands()
{
  printf(
        "Dicionário de comandos:\n"
        "/connect - conectar ao servidor\n"
        "/quit - sair da aplicação\n"
        "/ping - checar conexão com servidor\n"
        "/help - Exibe essa tela:\n\n"
        //"Comando: "
        );
}

int LSCommands(char* command)
{
  if(strcmp(command,"/join") == 0) {
    return JOIN;
  }
  else if (strcmp(command, "/nickname") == 0) {
    return NICKNAME;
  }
  else if(strcmp(command,"/ping") == 0) {
    return PING;
  }
  else if(strcmp(command,"/kick") == 0) {
    return KICK;
  }
  else if(strcmp(command,"/mute") == 0) {
    return MUTE;
  }
  else if(strcmp(command,"/unmute") == 0) {
    return UNMUTE;
  }
  else if (strcmp(command, "/whois") == 0){
    return WHOIS;
  }
  else if (strcmp(command, "/connect") == 0) {
    return CONNECT;
  }
  else if(strcmp(command, "/help") == 0){
    return HELP;
  }
  else if(strcmp(command, "/quit") == 0) {
    return QUIT;
  }

  return -1;
}

int ExecuteCommand(char* command, char* nickname, int permissions, char* IP) //change for stack?
{
    int aux_counter = 0;
    /* Breaks string using the big bad guy of string functions.
    May god have mercy on my soul for this
    */
    char new_nickname[4096];
    char* token;
    token = strtok(command, " ");
    while(token != NULL)
    {
        if (aux_counter == 0){
            aux_counter++;
            sprintf(new_nickname,"%s",token);
        }
        token = strtok(NULL," ");
    }
    new_nickname[(int)strlen(new_nickname)] = '\0';


  int selection = LSCommands(command);

    switch(selection){
        case JOIN:

        break;

        case NICKNAME:
            *nickname = new_nickname;
            printf("Nickname mudado para %s",new_nickname);
        break;

        case PING:
        break;

        case KICK:
          (permissions == 1)? printf("Vá pra casa, %s, você está bêbado(a)\n", new_nickname): printf("Você não possui permissão para kickar este membro");
        break;

        case MUTE:
        break;

        case UNMUTE:
        break;

        case WHOIS:
          if(permissions == 1) {
            printf("IP de %s: %s\n",new_nickname, IP);
          } else {
            printf("Você não possui permissão para ver o IP deste usuário");
          }
        break;

        case HELP:
          PrintComands();
        break;

        case CONNECT:
        return CONNECT;
        break;

        case QUIT:
        return QUIT;
        break;

        default:
            printf("Invalid command\n");
            return -1;
        break;
    }

  return 0;
}