#ifndef CLIENTPLAYER_H
#define CLIENTPLAYER_H

///LIBRAIRIES
#include <stdio.h>
#include <stdlib.h>
#include<string.h> //strlen
#include <winsock2.h> //to use sockets on Windows
#include <pthread.h>
//#include<sys/socket.h>
//#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write

///CONSTANT


///TYPES
typedef int socklen_t;

///PROTOTYPES
void client();
char* sendNOK();
char* sendConnect(char* name);
char* sendNewMove(int x, int y);
char* sendNOK();
int msgParserCP(int *msg, int WP);
void clientWP();


#endif

