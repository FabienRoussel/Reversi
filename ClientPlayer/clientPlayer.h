#ifndef CLIENTPLAYER_H
#define CLIENTPLAYER_H

///LIBRAIRIES
#include <stdio.h>
#include <stdlib.h>
#include<string.h> //strlen
#include <winsock2.h> //to use sockets on Windows
#include <pthread.h>
#include <errno.h>
//#include<sys/socket.h>
//#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write*
#include "ArtificialIntelligence.h"

///CONSTANT
#define TAB_LARG 8
#define TAB_HAUT 8
#define BOARD_SIZE 16


///TYPES
typedef int socklen_t;


///PROTOTYPES
Coords* CoordsTabToCoords(CoordsTab* coords);
CoordsTab* CoordsToCoordsTab(Coords* coords);

//Movement

int* encodeBoard(int boardState[TAB_LARG][TAB_HAUT]);
void client();

char* sendConnect(char* name);
char* sendNewMove(int x, int y);
char* sendNOK();
int msgParserCP(int *msg, int WP);
char** getBoard(char serverLength[2000]);
void clientWP();


#endif

