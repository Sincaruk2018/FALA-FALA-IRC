#ifndef JOIN
#define JOIN 0
#endif

#ifndef NICKNAME
#define NICKNAME 1
#endif

#ifndef PING
#define PING 2
#endif

#ifndef KICK
#define KICK 3
#endif

#ifndef MUTE
#define MUTE 4
#endif

#ifndef UNMUTE
#define UNMUTE 5
#endif

#ifndef WHOIS
#define WHOIS 6
#endif

#ifndef CONNECT
#define CONNECT 7
#endif

#ifndef HELP
#define HELP 8
#endif

#ifndef QUIT
#define QUIT 9
#endif

void PrintComands();

int LSCommands(char* command); // LS stands for Linear search

int ExecuteCommand(char* command, char* nickname, int permissions, char* IP);