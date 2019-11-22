#include "ArtificialIntelligence.h"

int predPossibleMove = 0;


/**
How to optmize my code ?
Work with char instead of int
recode my tab in int into char
recode isMyMoveValid into updateMatrix
recode PossibleMove
 eliminer les doublons !!
*/

int displayScore(int** board){
    int i, j, cntB=0, cntW=0;
    ///display(board);
    //we compute the number of pawn
    for(i=0; i<TAB_HAUT; i++){
        for(j=0; j<TAB_LARG; j++){
            if(board[i][j] == 1)
                cntB++;
            else if(board[i][j] == 10)
                cntW++;
        }
    }
 //   printf("Score : Black : %d, White : %d\n", cntB, cntW);
    return 64-cntB-cntW+1 ;
}


/**

rajouter un pré traitement avant le min max
     faire un coup direct si l'adversaire ne peut pas jouer le coup suivant,

post min max
    ajouter une règle pour éviter de poser un pion sur le coin (les trois cases) d"un coin


*/

int evaluateSituationMax(int** board, char colorPawn){
    int i, j, cntB=0, cntW=0, heuristic=0;
    ///display(board);
    //we compute the number of pawn
    for(i=0; i<TAB_HAUT; i++){
        for(j=0; j<TAB_LARG; j++){
            if(board[i][j] == 1)
                cntB++;
            else if(board[i][j] == 10)
                cntW++;
        }
    }
    heuristic = (colorPawn=='b')?((cntB-cntW)*1000/(cntB+cntW)) : ((cntW-cntB)*1000/(cntB+cntW));
///    printf("%d ", heuristic);

    float pointCornerW=0, pointCornerB=0;
    int coefA=5500, coefB=0;
    int matricePoint[TAB_HAUT][TAB_LARG] =
                         {  { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefA } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefB } };
    coefB= -9500;
    for(i=0; i<TAB_HAUT; i+=TAB_HAUT-1){
        for(j=0; j<TAB_LARG; j+=TAB_LARG-1){
            if(i==0 && j==0 && board[i][j]==0){
                matricePoint[i][j+1]=coefB;
                matricePoint[i+1][j]=coefB;
                matricePoint[i+1][j+1]=coefB;
            }
            else if(i==0 && j==TAB_LARG && board[i][j]==0){
                matricePoint[i][j-1]=coefB;
                matricePoint[i+1][j]=coefB;
                matricePoint[i+1][j-1]=coefB;
            }
            else if(i==TAB_HAUT && j==0 && board[i][j]==0){
                matricePoint[i][j+1]=coefB;
                matricePoint[i-1][j]=coefB;
                matricePoint[i-1][j+1]=coefB;
            }
            else if(i==TAB_HAUT && j==TAB_LARG && board[i][j]==0){
                matricePoint[i][j-1]=coefB;
                matricePoint[i-1][j]=coefB;
                matricePoint[i-1][j-1]=coefB;
            }
        }
    }
    //we compute the difference en posseded corners
    for(i=0; i<TAB_HAUT; i++){
        for(j=0; j<TAB_LARG; j++){
            if(board[i][j] == 1)
                pointCornerW += matricePoint[i][j];
            else if(board[i][j] == 10)
                pointCornerB += matricePoint[i][j];
        }
    }
    if(pointCornerW+pointCornerB != 0)
        heuristic += (colorPawn=='b')?
            (float)((pointCornerB-pointCornerW)/(pointCornerB+pointCornerW)) :
                (float)((pointCornerW-pointCornerB)/(pointCornerB+pointCornerW)) ;
    return heuristic;
}

int evaluateSituationMin(int** board, char colorPawn){
    int i, j, cntB=0, cntW=0, heuristic=0;

    //we compute the number of pawn
    for(i=0; i<TAB_HAUT; i++){
        for(j=0; j<TAB_LARG; j++){
            if(board[i][j] == 1)
                cntB++;
            else if(board[i][j] == 10)
                cntW++;
        }
    }
    heuristic = (colorPawn=='b')? ((cntW-cntB)*1000/(cntB+cntW)) : ((cntB-cntW)*1000/(cntB+cntW));

    float pointCornerW=0, pointCornerB=0;
    int coefA=5500, coefB=0;
    int matricePoint[TAB_HAUT][TAB_LARG] =
                         {  { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefA } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefB } };
    coefB= -9500;
     for(i=0; i<TAB_HAUT; i+=TAB_HAUT-1){
        for(j=0; j<TAB_LARG; j+=TAB_LARG-1){
             if(i==0 && j==0 && board[i][j]==0){
                matricePoint[i][j+1]=coefB;
                matricePoint[i+1][j]=coefB;
                matricePoint[i+1][j+1]=coefB;
            }
            else if(i==0 && j==TAB_LARG && board[i][j]==0){
                matricePoint[i][j-1]=coefB;
                matricePoint[i+1][j]=coefB;
                matricePoint[i+1][j-1]=coefB;
            }
            else if(i==TAB_HAUT && j==0 && board[i][j]==0){
                matricePoint[i][j+1]=coefB;
                matricePoint[i-1][j]=coefB;
                matricePoint[i-1][j+1]=coefB;
            }
            else if(i==TAB_HAUT && j==TAB_LARG && board[i][j]==0){
                matricePoint[i][j-1]=coefB;
                matricePoint[i-1][j]=coefB;
                matricePoint[i-1][j-1]=coefB;
            }
        }
    }
    //we compute the difference en posseded corners
    for(i=0; i<TAB_HAUT; i++){
        for(j=0; j<TAB_LARG; j++){
            if(board[i][j] == 1)
                pointCornerW += matricePoint[i][j];
            else if(board[i][j] == 10)
                pointCornerB += matricePoint[i][j];
        }
    }
    if(pointCornerW+pointCornerB != 0)
        heuristic += (colorPawn=='b')?
            (float)((pointCornerW-pointCornerB)/(pointCornerB+pointCornerW)) :
                (float)((pointCornerB-pointCornerW)/(pointCornerB+pointCornerW)) ;

    return heuristic;
}

void updateMatrix(int** board, char colorPawn, Coords* coord){
    int i, indexBoard=1;
    //table of changing case
    Coords boardChangeCellValue[62];
    MatrixCoordinates* boardIntermed;
    boardChangeCellValue[0] = *coord;

    dir direction;
    // we save the coordinates
    int lig = coord->x, col=coord->y;
    Coords* newPos = (Coords*)malloc(sizeof(Coords));
    newPos->x=lig;
    newPos->y=col;// we check for each of the 8 directions
    for(direction=UP_LEFT; direction<=DOWN_RIGHT; direction++){
        newPos = actuCoord(newPos, direction); // compute newPOs
        boardIntermed=isCellsOccupiedByAdversary(board, colorPawn, newPos, direction);
        for(i=0; i<boardIntermed->numberOfCoord; i++){
            boardChangeCellValue[indexBoard].x=boardIntermed->matrix[i]->x;
            boardChangeCellValue[indexBoard].y=boardIntermed->matrix[i]->y;
            indexBoard++;
        }
        freeMatrixCoordinate(boardIntermed);
        //we put the anciants coordinate
        newPos->x=lig;
        newPos->y=col;
    }
    for(i=0; i<indexBoard; i++){
        board[boardChangeCellValue[i].x][boardChangeCellValue[i].y]=(colorPawn=='b')? 01 : 10;
    }
    free(newPos);
}

int MinMax(unsigned short depth, Node* nodeActu, int** board, char colorPawn, int alpha, int beta)
{
    int moreRandom=rand()%2;
    //if the node is a leaf, this means depth max is attained or game is over
    // then score is equal to function of evaluation
    if (depth==0 /* || gameOver()*/)
    {
        nodeActu->nextNodes = NULL;
        if (nodeActu->grade==-INT_MAX){
            nodeActu->grade = evaluateSituationMax(board, colorPawn);
            return nodeActu->grade;
        }

        else {
                nodeActu->grade = evaluateSituationMin(board, colorPawn);
                return nodeActu->grade;
        }
        // fonction d'évaluation au lieu de 0
        return 0;
    }
    MatrixCoordinates* possibleMoveCoords;
    possibleMoveCoords = possibleMove(board, colorPawn);
    if(possibleMoveCoords->numberOfCoord>0){
        int value;
        if(nodeActu->grade==-INT_MAX){
            int i, nbrCoords = possibleMoveCoords->numberOfCoord;
            int boardSave[TAB_HAUT][TAB_LARG];
            int j, k;
            for(k=0; k< TAB_HAUT; k++){
                for(j=0; j< TAB_LARG; j++)
                    boardSave[k][j] = board[k][j];
            }
            predPossibleMove = nbrCoords;
            nodeActu->nextNodes = (Node**)malloc(sizeof(Node*)*nbrCoords);
            for(i=0; i< nbrCoords; i++){
                updateMatrix(board, colorPawn, possibleMoveCoords->matrix[(moreRandom? i : nbrCoords-1 -i)]);
                nodeActu->nextNodes[i] = (Node*)malloc(sizeof(Node));
                nodeActu->nextNodes[i]->grade = - nodeActu->grade;
                nodeActu->nextNodes[i]->myMove = (Coords*)malloc(sizeof(Coords));
                nodeActu->nextNodes[i]->myMove->x = possibleMoveCoords->matrix[(moreRandom? i : nbrCoords-1 -i)]->x;
                nodeActu->nextNodes[i]->myMove->y = possibleMoveCoords->matrix[(moreRandom? i : nbrCoords-1 -i)]->y;
                value = MinMax(depth-1, nodeActu->nextNodes[i], board, (colorPawn=='b')? 'w' : 'b', alpha, beta);
                for(k=0; k< TAB_HAUT; k++){
                    for(j=0; j< TAB_LARG; j++)
                        board[k][j] = boardSave[k][j];
                }
                nodeActu->nextNodes[i]->grade = value;
                if(value > alpha){
                    alpha = value;
                    if(alpha>=beta){
                        break;
                    }
                }
            }
            nodeActu->successors = i;
            nodeActu->grade = maxNode(nodeActu->nextNodes, i) ;
            freeMatrixCoordinate(possibleMoveCoords);
            return alpha;
        }
        else if(nodeActu->grade==INT_MAX){
            int i, nbrCoords = possibleMoveCoords->numberOfCoord;
            int boardSave[TAB_HAUT][TAB_LARG];
            int j, k;
            for(k=0; k< TAB_HAUT; k++){
                for(j=0; j< TAB_LARG; j++)
                    boardSave[k][j] = board[k][j];
            }
            predPossibleMove = nbrCoords;
            nodeActu->nextNodes = (Node**)malloc(sizeof(Node*)*nbrCoords);
            for(i=0; i< nbrCoords; i++){
                updateMatrix(board, colorPawn, possibleMoveCoords->matrix[(moreRandom? i : nbrCoords-1 -i)]);
                nodeActu->nextNodes[i] = (Node*)malloc(sizeof(Node));
                nodeActu->nextNodes[i]->grade = - nodeActu->grade;
                nodeActu->nextNodes[i]->myMove = (Coords*)malloc(sizeof(Coords));
                nodeActu->nextNodes[i]->myMove->x = possibleMoveCoords->matrix[(moreRandom? i : nbrCoords-1 -i)]->x;
                nodeActu->nextNodes[i]->myMove->y = possibleMoveCoords->matrix[(moreRandom? i : nbrCoords-1 -i)]->y;
                value = MinMax(depth-1, nodeActu->nextNodes[i], board, (colorPawn=='b')? 'w' : 'b', alpha, beta);
                for(k=0; k< TAB_HAUT; k++){
                    for(j=0; j< TAB_LARG; j++)
                        board[k][j] = boardSave[k][j];
                }
                nodeActu->nextNodes[i]->grade = value;
                if(value < beta){
                    beta = value;
                    if(alpha>=beta){
                        break;
                    }

                }
            }
            nodeActu->successors = i;
            nodeActu->grade = minNode(nodeActu->nextNodes, i);
            freeMatrixCoordinate(possibleMoveCoords);
            return beta;
        }
    }
    else
        freeMatrixCoordinate(possibleMoveCoords);

    //Sinon, si les fils du noeud sont des coups jouable par le joueur (l'IA),
    //alors le noeud est un « noeud joueur » et son score est le maximum du score des fils.

    //Sinon, les fils du noeuds sont des coups jouables par l'adversaire,
    //le noeud est donc un « noeud adverse » et son score est le minimum du score des fils.
    return 0;
}

int minNode(Node** nodes, int nbrNode){
    int i, min=INT_MAX;
    for(i=0; i< nbrNode; i++){
        if (nodes[i]->grade < min)
            min = nodes[i]->grade;
    }
    return min;
}
int maxNode(Node** nodes, int nbrNode){
    int i, max=-INT_MAX;
    for(i=0; i< nbrNode; i++){
        if (nodes[i]->grade > max)
            max = nodes[i]->grade;
    }
    return max;
}

void displayTree(Node* root){
    if(root->nextNodes!= NULL){
        int i;
        printf("\n\n");
        printf("\t\t %d\n", root->grade);
        for(i=0; i<root->successors; i++){
            printf("\t%d", root->nextNodes[i]->grade);
        }
        for(i=0; i<root->successors; i++){
            displayTree(root->nextNodes[i]);
        }
    }
}

void freeTree(Node* root){
    if(root->nextNodes!= NULL){
        int i;
        for(i=0; i<root->successors; i++){
            freeTree(root->nextNodes[i]);
        }
        free(root->myMove);
        free(root);
    }
    else{
        free(root->myMove);
        free(root);
    }
}

int numberPossibleMove(int** boardState, char colorPawn)
{
    int lig, col;
    //the  tempory board of move

    MatrixCoordinates* boardIntermed;
    // the number of Coord in the board
    int indexBoard=0;
    dir direction;
    // tempory coord
    Coords* position = (Coords*)malloc(sizeof(Coords));
    // we go through the matrix in line and columns
    for(lig=0; lig< TAB_LARG; lig++)
    {
        for(col=0; col< TAB_HAUT; col++)
        {
            // if the cell is free
            if(boardState[lig][col]==0)
            {
                // for each direction
                for(direction=UP_LEFT; direction<=DOWN_RIGHT; direction++)
                {
                    // we update the position
                    position->x=lig;
                    position->y=col;
                    // we compute the coord of the cell that could change with the move
                    boardIntermed=isCellsOccupiedByAdversary(boardState, colorPawn, actuCoord(position, direction), direction);
                    // and if the matrix returned is not null
                    if(boardIntermed->numberOfCoord>0)
                    {
                        freeMatrixCoordinate(boardIntermed);
                        indexBoard++; // we increment the number of possible move
                        break; // we change direction because if the move is posssible for one direction
                        // then it means we can play it
                    }
                    else
                        freeMatrixCoordinate(boardIntermed);
                }
            }
        }
    }
    free(position);

    return indexBoard;
}

void freeMatrixCoordinate(MatrixCoordinates* matrix){
    int i;
    for (i=0; i< matrix->numberOfCoord; i++){
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    free(matrix);
}

void color(int flags)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,(WORD)flags);
}

void mettons_de_la_couleur(char ch)
{
    //0. Déclaration des ressources
    /// Toutes les variables utiles sont déclarées par le biais du tube

    //1. Attribution de la couleur en fonction de l'item reçu en paramètre
    if(ch=='_') color(FOREGROUND_RED ) ;/* | FOREGROUND_INTENSITY*/
    else if(ch=='B') color(FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
    else if(ch=='W') color(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY) ;
    //3. Affichage de l'item à l'écran
    printf("%c ", ch);

    //4. Retour de la couleur au blanc pour les autres affichages (contrat, nombre de coups restants)
    color(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED) ;
}

void display(int**tab)
{
    //variables
    int i,j;

    ///display the information about the game
    printf("ASCII Mode:\n");
    /*printf("Black Player: %s\n",black_player);
    printf("Last Move: %s\n",black_move);
    printf("Points: %d\n",black_pts);
    printf("Time: %d ms\n",black_time);
    printf("White Player: %s\n",white_player);
    printf("Last Move: %s\n",white_move);
    printf("Points: %d\n",white_pts);
    printf("Time: %d ms\n\n",white_time);*/

    ///display the game
    printf("    A B C D E F G H\n");

    for (i=0; i<TAB_HAUT; i++)
    {
        printf("%d   ", (i+1));
        for (j=0; j<TAB_LARG; j++)
        {
            mettons_de_la_couleur(cellIntToChar(tab[i][j]));
        }
        printf("\n");
    }
    printf("\n");
/*
    ///display commands
    printf("Commands:\n(s): Step by step mode\n");
    printf("(c): Continuous mode\n");
    printf("(ch): change board size\n");
    printf("(q): quit\n");
    printf("(r): restart\n");
    */
}
char cellIntToChar(int cell){
    switch (cell){
        case 00:
            return '_';
            break;
        case 01:
            return 'B';
            break;
        case 10:
            return 'W';
            break;
        default:
            return '?';
    }
    return '?';
}


/// @Definition:
///     find all possible move of a player
/// @Parameters :
///     the board of the game in int**, the color of the player in char 'b' for black et 'w' for white
/// @Output :
///     Coords ** of all possible move, number of line is number of move with x and y (position) each line
MatrixCoordinates* possibleMove(int** boardState, char colorPawn)
{
    int lig, col, i;
    //the  tempory board of move
    Coords* allPossibleMove = (Coords*) malloc(sizeof(Coords)*TAB_HAUT*TAB_LARG);
    MatrixCoordinates* boardIntermed;
    // the number of Coord in the board
    int indexBoard=0;
    dir direction;
    // tempory coord
    Coords* position = (Coords*)malloc(sizeof(Coords));
    // we go through the matrix in line and columns
    for(lig=0; lig< TAB_LARG; lig++)
    {
        for(col=0; col< TAB_HAUT; col++)
        {
            // if the cell is free
            if(boardState[lig][col]==0)
            {
                // for each direction
                for(direction=UP_LEFT; direction<=DOWN_RIGHT; direction++)
                {
                    // we update the position
                    position->x=lig;
                    position->y=col;
                    // we compute the coord of the cell that could change with the move
                    boardIntermed=isCellsOccupiedByAdversary(boardState, colorPawn, actuCoord(position, direction), direction);
                    // and if the matrix returned is not null
                    if(boardIntermed->numberOfCoord>0)
                    {
                        // we copy the actual position in the board of Possible move
                        allPossibleMove[indexBoard].x = lig;
                        allPossibleMove[indexBoard].y = col;
                        indexBoard++; // we increment the number of possible move
                        break; // we change direction because if the move is posssible for one direction
                        // then it means we can play it
                    }
                    freeMatrixCoordinate(boardIntermed);
                }
            }
        }
    }
    // we allocate a new matrix of Coord in order to take less memory
    MatrixCoordinates* myMatrix= (MatrixCoordinates*)malloc(sizeof(MatrixCoordinates));
    myMatrix->matrix = (Coords**)malloc(sizeof(Coords*)*indexBoard);
    for(i=0; i<indexBoard; i++)
    {
        myMatrix->matrix[i] = (Coords*)malloc(sizeof(Coords));
        myMatrix->matrix[i]->x = allPossibleMove[i].x;
        myMatrix->matrix[i]->y = allPossibleMove[i].y;
/// line to suppress or to put ailleurs
      //  printf("%c %d is playable \n", myMatrix->matrix[i]->y +'A' , myMatrix->matrix[i]->x+1);
    }
    free(position);
    free(allPossibleMove);
    myMatrix->numberOfCoord = indexBoard;
    return myMatrix;
}


// return 1 if the cell is out of the matrix
int isCellOutOfBound(Coords* coord)
{
    return (coord->x>TAB_LARG-1 || coord->x<0 || coord->y>TAB_HAUT-1 || coord->y<0);
}

Coords* actuCoord(Coords* coord, dir direction)
{
    switch(direction)
    {
    case UP_LEFT:
        coord->x-=1;
        coord->y-=1;
        break;
    case UP:
        coord->x-=1;
        break;
    case UP_RIGHT:
        coord->x-=1;
        coord->y+=1;
        break;
    case LEFT:
        coord->y-=1;
        break;
    case RIGHT:
        coord->y+=1;
        break;
    case DOWN_LEFT:
        coord->x+=1;
        coord->y-=1;
        break;
    case DOWN:
        coord->x+=1;
        break;
    case DOWN_RIGHT:
        coord->x+=1;
        coord->y+=1;
        break;
    default:
        printf("Bad direction");
        break;
    }
    return coord;
}

//return a tab of cell
MatrixCoordinates* isCellsOccupiedByAdversary(int** boardState, char colorPawn, Coords* coord, dir direction)
{
    //ID of the players
    int adversary = (colorPawn=='b')? 10 : 01;
    int player = (colorPawn=='b')? 01 : 10;
    // counter of cell in the tab we return
    int cptCellAdver = 0, i;

    //the first cell of this matrix represents the number of next cell in the matrix
    // example if cell[0]->x is egal to 5, we have to read the cell 1 to 4 included;
    MatrixCoordinates* boardChangeCellValue = (MatrixCoordinates*) malloc(sizeof(MatrixCoordinates));
    Coords BoardIntermed[TAB_LARG-1];

    Coords *newPos = coord;
    do
    {
        // if cell not out of bound
        if (!isCellOutOfBound(newPos))
        {
            // if next cell is an adversary
            if (boardState[newPos->x][newPos->y]==adversary)
            {
                // we add the adverse cell in the tab to actualize and increment the counter of cell
                BoardIntermed[cptCellAdver].x=newPos->x;
                BoardIntermed[cptCellAdver].y=newPos->y;
                cptCellAdver++;
                newPos = actuCoord(newPos, direction); // compute newPOs
            }// else we check if the next cell is our color and if the counter has counted at least one advers cell (initialy the counter is at 1 so if it is more tahn 1)
            else if (boardState[newPos->x][newPos->y]==player && cptCellAdver>0)
            {
                boardChangeCellValue->matrix = (Coords**) malloc(sizeof(Coords*)*cptCellAdver);
                for(i=0; i<cptCellAdver ; i++){
                    boardChangeCellValue->matrix[i] =(Coords*) malloc(sizeof(Coords));
                    boardChangeCellValue->matrix[i]->x = BoardIntermed[i].x;
                    boardChangeCellValue->matrix[i]->y = BoardIntermed[i].y;
                }
                boardChangeCellValue->numberOfCoord=cptCellAdver; //marker end
                return boardChangeCellValue;// we return the tab
            }
            else
            {
                boardChangeCellValue->matrix = (Coords**) malloc(sizeof(Coords*));
                boardChangeCellValue->numberOfCoord=0; //marker end
                return boardChangeCellValue;// we return the tab
            }
        }
        else
        {
            boardChangeCellValue->matrix = (Coords**) malloc(sizeof(Coords*));
            // we are out of bounds, not valid return end marker
            boardChangeCellValue->numberOfCoord=0; //marker end
            return boardChangeCellValue;// we return the tab
        }
    }
    while(1);
    //error
    boardChangeCellValue->numberOfCoord=0;
    return boardChangeCellValue;
}

/// @Definition:
///     find if a play is valid
/// @Parameters :
///     the board of the game in int**, the color of the player in char 'b' for black et 'w' for white
///     and the coordinate of the desired move
/// @Output :
///     1 if the move is possible, else 0
int isMyMoveValid(int** boardState, char colorPawn, Coords* coord)
{
    //if the coordinate are not  valid leave
    if (isCellOutOfBound(coord)) return 0;
    // if the cell is not empty, move is not valid
    if (boardState[coord->x][coord->y]!=0) return 0;

    int i, indexBoard=1;
    MatrixCoordinates* boardIntermed;
    //table of changing case
    Coords boardChangeCellValue[62];
    boardChangeCellValue[0] = *coord;
    for(i=1; i <63; i++)  /// this few lines of for aren't mandatory // we could supress them
    {
        boardChangeCellValue[i].x=-1;
        boardChangeCellValue[i].y=-1;
    }
    //else we check for each of the 8 directions
    dir direction;
    // we save the coordinates
    int lig = coord->x, col=coord->y;
    Coords* newPos = (Coords*)malloc(sizeof(Coords));
    newPos->x=lig;
    newPos->y=col;
    for(direction=UP_LEFT; direction<=DOWN_RIGHT; direction++)
    {
        newPos = actuCoord(newPos, direction); // compute newPOs
//       printf("direction : %d, newPos: %d %d, pos: %d, %d\n", direction, newPos->x, newPos->y, coord->x, coord->y);
        boardIntermed=isCellsOccupiedByAdversary(boardState, colorPawn, newPos, direction);
        for(i=0; i<boardIntermed->numberOfCoord; i++)
        {
            boardChangeCellValue[indexBoard].x=boardIntermed->matrix[i]->x;
            boardChangeCellValue[indexBoard].y=boardIntermed->matrix[i]->y;
            indexBoard++;
        }
        freeMatrixCoordinate(boardIntermed);
        //we put the anciants coordinate
        newPos->x=lig;
        newPos->y=col;
    }
    if(indexBoard>1)
    {
        // update also the matrix
        int i;
        for(i=0; i<indexBoard; i++)
        {
            boardState[boardChangeCellValue[i].x][boardChangeCellValue[i].y]=(colorPawn=='b')? 01 : 10;
        }
        free(newPos);
        return 1;
    }
    return 0;
}
