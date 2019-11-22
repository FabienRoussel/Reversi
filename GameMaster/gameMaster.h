#ifndef GAMEMASTER_H
#define GAMEMASTER_H

///LIBRAIRIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <winsock2.h> //to use sockets on Windows
#include <unistd.h> //write
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO

///CONSTANT
#define BOARD_SIZE 16
#define TAB_LARG 8
#define TAB_HAUT 8
#define MAX_CLIENTS 3
#define EINTR 4

///TYPES
typedef int socklen_t;

/// 0 1 2 3 4 5 6 7
typedef struct Coords
{
    int x; // line
    int y; // column
}Coords;

/// 1 2 3 4 5 6 7 8 and A B C D E F G H
typedef struct CoordsTab
{
    int x; // line
    char y; // column
}CoordsTab;

typedef struct MatrixCoordinates
{
    int numberOfCoord;
    Coords** matrix;
}MatrixCoordinates;

typedef enum direction
{
    UP_LEFT=0, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT
}dir;

///PROTOTYPES
void server();

CoordsTab* CoordsToCoordsTab(Coords* coords);

void freeMatrixCoordinate(MatrixCoordinates* matrix);
//Movement

int numberPossibleMove(int** boardState, char colorPawn);
MatrixCoordinates* possibleMove(int** boardState, char colorPawn);
int isCellOutOfBound(Coords* coord);
Coords* actuCoord(Coords* coord, dir direction);
MatrixCoordinates* isCellsOccupiedByAdversary(int** boardState, char colorPawn, Coords* coord, dir direction);
int isMyMoveValid(int** boardState, char colorPawn, Coords* coord);
int IsAllCellsTaken(int** board);
char* sendStatus1(int lastX, int lastY, char boardState[TAB_HAUT][TAB_LARG]);

int* encodeBoard(int boardState[TAB_LARG][TAB_HAUT]);
struct sockaddr_in setParameters(int port);
int binding(int sock, struct sockaddr_in server);
int moveChecker(int x, int y, int WP);
char* sendOK();
char* sendNOK();
char* sendEND();
char* sendNextTurn(int lastX, int lastY, char boardState[TAB_HAUT][TAB_LARG]);
int msgParserGM(int *msg, int WP);
void server();
void createServer();

#endif
