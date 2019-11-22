#ifndef GAMECONTROLER_H
#define GAMECONTROLER_H

///LIBRAIRIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <winsock2.h> //to use sockets on Windows
#include <math.h>
#include <pthread.h>
#include <unistd.h> //write
#include "gameControler.h"


///CONSTANT
#define TAB_LARG 8
#define TAB_HAUT 8
#define BOARD_SIZE 16

///TYPES
typedef int socklen_t;

///PROTOTYPES
void color(int flags);
void mettons_de_la_couleur(char ch);
int decimalToBinary(int n);
int* divideBinaryIntoInt(int decimal);
char* decimalToHexa(int decimal);
int binaryToDecimal(int binary);
int hexaToDecimal(char* hexa);
int hexaToBinary(char* hexa);
char* binaryToHexa(int binary);
char* sendStatus1(int lastX, int lastY, int boardState[BOARD_SIZE]);
char* sendStatus2(int ptBP, int timerBP,char *nameBP);
char* sendConnect(char* name);
char cellIntToChar(int cell);
void display(char**tab);
char** fillTab(char** tab);
char** decodeBoard(int player,char serverLength[2000]);
void game();
void gameLoop();



#endif
