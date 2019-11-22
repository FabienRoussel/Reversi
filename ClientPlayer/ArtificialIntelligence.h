#ifndef ARTIFICIALINTELLIGENCE_H_INCLUDED
#define ARTIFICIALINTELLIGENCE_H_INCLUDED
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#define TAB_LARG 8
#define TAB_HAUT 8
/// 1 2 3 4 5 6 7 8 and A B C D E F G H
typedef struct CoordsTab
{
    int x; // line
    char y; // column
}CoordsTab;

/// 0 1 2 3 4 5 6 7
typedef struct Coords
{
    int x; // line
    int y; // column
}Coords;

typedef struct MatrixCoordinates
{
    int numberOfCoord;
    Coords** matrix;
}MatrixCoordinates;


typedef enum direction
{
    UP_LEFT=0, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT
}dir;

//Struct
typedef struct Node{
    int grade;
    Coords* myMove;
    unsigned short successors;
    struct Node** nextNodes;
}Node;


MatrixCoordinates* possibleMove(int** boardState, char colorPawn);
int isCellOutOfBound(Coords* coord);
Coords* actuCoord(Coords* coord, dir direction);
MatrixCoordinates* isCellsOccupiedByAdversary(int** boardState, char colorPawn, Coords* coord, dir direction);
int isMyMoveValid(int** boardState, char colorPawn, Coords* coord);


// function
/// return if it is the end of the game
int displayScore(int** board);
int evaluateSituationMax(int** board, char colorPawn);
int evaluateSituationMin(int** board, char colorPawn);
int MinMax(unsigned short depth, Node* nodeActu, int** board, char colorPawn, int alpha, int beta);
int minNode(Node** nodes, int nbrNode);
int maxNode(Node** nodes, int nbrNode);
void freeTree(Node* root);
void displayTree(Node* root);
int numberPossibleMove(int** boardState, char colorPawn);

void freeMatrixCoordinate(MatrixCoordinates* matrix);
void color(int flags);
void mettons_de_la_couleur(char ch);
void display(int**tab);
char cellIntToChar(int cell);
#endif // ARTIFICIALINTELLIGENCE_H_INCLUDED
