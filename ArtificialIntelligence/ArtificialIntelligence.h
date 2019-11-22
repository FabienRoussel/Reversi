#ifndef ARTIFICIALINTELLIGENCE_H_INCLUDED
#define ARTIFICIALINTELLIGENCE_H_INCLUDED
#include "gameControler.h"


//Struct
typedef struct Node{
    int grade;
    Coords* myMove;
    unsigned short successors;
    struct Node** nextNodes;
}Node;

// function
/// return if it is the end of the game
int displayScore(int** board);
int test(int** board);
int evaluateSituationMax(int** board, char colorPawn);
int evaluateSituationMin(int** board, char colorPawn);
void createJarvis(int** board);
int MinMax(unsigned short depth, Node* nodeActu, int** board, char colorPawn, int alpha, int beta);
int minNode(Node** nodes, int nbrNode);
int maxNode(Node** nodes, int nbrNode);
void freeTree(Node* root);
void displayTree(Node* root);
int numberPossibleMove(int** boardState, char colorPawn);

void freeMatrixCoordinate(MatrixCoordinates* matrix);
Coords* choseAGoodMove(int** board, char colorPawn);
void color(int flags);
void mettons_de_la_couleur(char ch);
#endif // ARTIFICIALINTELLIGENCE_H_INCLUDED
