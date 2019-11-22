#include "ArtificialIntelligence.h"
#include <time.h>

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

int isPawnUntakable(int** board, MatrixCoordinates* movePossible){
    int i;
    for (i=0; i<movePossible->numberOfCoord; i++){
        if(movePossible->matrix[i]->x ==0){
            if(movePossible->matrix[i]->y ==0 || movePossible->matrix[i]->y == TAB_LARG-1)
                return i;
        }
        else if(movePossible->matrix[i]->x ==TAB_HAUT-1){
            if(movePossible->matrix[i]->y ==0 || movePossible->matrix[i]->y == TAB_LARG-1)
                return i;
        }
    }
    return 64;
}

int test(int** board){
    Node* node;
    Coords* coord = (Coords*)malloc(sizeof(Coords));
    //CoordsTab* coordInTab = (CoordsTab*)malloc(sizeof(CoordsTab));
    MatrixCoordinates* movePossible;
    char c = 'b';
    //int validMove=0;
    int depth=6, firstDept=6, goodMove;
    int j=0, i=0, playerHasNotPlay=0;
    do{
        fflush(stdin);
        if (c == 'b'){
            movePossible=possibleMove(board, c);
            if(movePossible->numberOfCoord>0){
                coord->x = movePossible->matrix[0]->x ;
                coord->y = movePossible->matrix[0]->y ;
                if(0){
 //               if((goodMove=isPawnUntakable(board, movePossible))!=64){
                    coord->x = movePossible->matrix[goodMove]->x ;
                    coord->y = movePossible->matrix[goodMove]->y ;
                    isMyMoveValid(board, c, coord);
                }
                else{
                    node = (Node*)malloc(sizeof(Node));
                    node->grade = -INT_MAX;
                    MinMax(depth, node, board, c, -INT_MAX, INT_MAX);
                    for(i=0; i<node->successors; i++){
                        if (node->grade == node->nextNodes[i]->grade){
                            coord = node->nextNodes[i]->myMove;
                            break;
                        }
                    }
                    isMyMoveValid(board, c, coord);
                }
                printf("Turn %c\n", c);
                display(board);
                j++;
 ///               Sleep(10);
                playerHasNotPlay=0;
            }
            else playerHasNotPlay++;
            freeMatrixCoordinate(movePossible);
            c = c=='b'? 'w' : 'b';
        }
        else{
            coord = choseAGoodMove(board, c);
            if(coord->x > -1){
                isMyMoveValid(board, c, coord);
                printf("Turn %c\n", c);
                display(board);
                j++;
                playerHasNotPlay=0;
            }
            else playerHasNotPlay++;
            c = c=='b'? 'w' : 'b';
/*
            movePossible=possibleMove(board, c);
            if(movePossible->numberOfCoord>0){
                do{
                    printf("Your move: ");
                    fflush(stdin);
                    scanf("%c", &coordInTab->y);
                    scanf("%d", &coordInTab->x);
                    coord = CoordsTabToCoords(coordInTab);
                 //   coord->x = rand()%TAB_HAUT;
               //     coord->y = rand()%TAB_LARG;
                    printf("%d, %d ", coord->x, coord->y);
                    if( (validMove=isMyMoveValid(board, c, coord)) == 1 ){
                        printf("Turn %c\n", c);
                        display(board);
                        j++;
                        c = c=='b'? 'w' : 'b';
                        Sleep(10);
                    }
                }while(!validMove);
            }else c = c=='b'? 'w' : 'b';
            freeMatrixCoordinate(movePossible);*/
        }

        if (j==55){
            j=0;
    ///        system("PAUSE");
        }
        if ((depth=displayScore(board)-1) > firstDept) depth = firstDept;
    }while(depth && playerHasNotPlay!=2);
    free(coord);

    int cntB=0, cntW=0;
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
    //printf("Score : Black : %d, White : %d\n", cntB, cntW);
    if (cntB-cntW >20 || cntW-cntB>20) display(board);
    if (cntB>cntW) return 1;
    return 0;
}
/*
void createJarvis(int** board){
    printf("Press a button when you are ready to start\n");
    system("PAUSE");
    Node* node = (Node*)malloc(sizeof(Node));
    node->grade = -INT_MAX;
    MinMax(2, node, board, 'w', -INT_MAX, INT_MAX);
    displayTree(node);
    freeTree(node);
    printf("\n\n");
    Sleep(800000);
}*/
/*
int evaluateSituationMax(int** board, char colorPawn){
    int i, j, cntB=0, cntW=0, cornerW=0, cornerB=0, heuristic=0;
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
    // we compute the mobility
    int nbrMoov = numberPossibleMove(board, colorPawn);
    heuristic += (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
         ((nbrMoov-predPossibleMove)*100/(nbrMoov+predPossibleMove))
        : ((predPossibleMove-nbrMoov)*100/(nbrMoov+predPossibleMove)) ));
///    printf("+ %d  = ", (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
///         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
///        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) )));

    float pointCornerW=0, pointCornerB=0;
    int coefA=2500, coefB=0;
    int matricePoint[TAB_HAUT][TAB_LARG] =
                         {  { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefA } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefB } };
    coefB= -1500;
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
    int i, j, cntB=0, cntW=0, cornerW=0, cornerB=0, heuristic=0;

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

    // we compute the mobility
    int nbrMoov = numberPossibleMove(board, colorPawn);
    heuristic += (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) ));

    float pointCornerW=0, pointCornerB=0;
    int coefA=3500, coefB=0;
    int matricePoint[TAB_HAUT][TAB_LARG] =
                         {  { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefA } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            { 0     ,   0   ,  0  , 0 , 0 , 0   , 0     ,   0   } ,
                            {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                            { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                            { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefB } };
    coefB= -2500;
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
}*/
int evaluateSituationMax(int** board, char colorPawn){
    int i, j, cntB=0, cntW=0, cornerW=0, cornerB=0, heuristic=0;
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
    // we compute the mobility
    int nbrMoov = numberPossibleMove(board, colorPawn);
    heuristic += (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) ));
///    printf("+ %d  = ", (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
///         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
///        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) )));

    cntB=0; cntW=0;
    //we compute the difference en posseded corners
    for(i=0; i<TAB_HAUT; i+=TAB_HAUT-1){
        for(j=0; j<TAB_LARG; j+=TAB_LARG-1){
            if(board[i][j] == 1)
                cornerB++;
            else if(board[i][j] == 10)
                cornerW++;// 01 06   10 11 16 17     60 61 66 67   71 76
            else if(i==0 && j==0){
                cntB = (board[i][j+1]==10) + (board[i+1][j]==10) + (board[i+1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i+1][j]==01) + (board[i+1][j+1]==01);
            }
            else if(i==0 && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i+1][j]==10) + (board[i+1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i+1][j]==01) + (board[i+1][j-1]==01);
            }
            else if(i==TAB_HAUT && j==0){
                cntB = (board[i][j+1]==10) + (board[i-1][j]==10) + (board[i-1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i-1][j]==01) + (board[i-1][j+1]==01);
            }else if(i==TAB_HAUT && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i-1][j]==10) + (board[i-1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i-1][j]==01) + (board[i-1][j-1]==01);
            }
        }
    }
    if(cornerB+cornerW != 0)
        heuristic += (colorPawn=='b')?
            (float)((cornerB-cornerW)*1500/(cornerB+cornerW)) : (float)((cornerW-cornerB)*1500/(cornerB+cornerW));
    if(cntB+cntW != 0)
        heuristic -= (colorPawn=='b')?
            (float)((cntB-cntW)*500/(cntB+cntW)) : (float)((cntW-cntB)*500/(cntB+cntW));
    return heuristic;
}

int evaluateSituationMin(int** board, char colorPawn){
    int i, j, cntB=0, cntW=0, cornerW=0, cornerB=0, heuristic=0;

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

    // we compute the mobility
    int nbrMoov = numberPossibleMove(board, colorPawn);
    heuristic += (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) ));

    cntB=0; cntW=0;
///TODO: we need to add a negative coefficient for each cell near the corner if the corner is not  taken
    //we compute the difference en posseded corners
    for(i=0; i<TAB_HAUT; i+=TAB_HAUT-1){
        for(j=0; j<TAB_LARG; j+=TAB_LARG-1){
            if(board[i][j] == 1)
                cornerB++;
            else if(board[i][j] == 10)
                cornerW++;// 01 06   10 11 16 17     60 61 66 67   71 76
            else if(i==0 && j==0){
                cntB = (board[i][j+1]==10) + (board[i+1][j]==10) + (board[i+1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i+1][j]==01) + (board[i+1][j+1]==01);
            }
            else if(i==0 && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i+1][j]==10) + (board[i+1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i+1][j]==01) + (board[i+1][j-1]==01);
            }
            else if(i==TAB_HAUT && j==0){
                cntB = (board[i][j+1]==10) + (board[i-1][j]==10) + (board[i-1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i-1][j]==01) + (board[i-1][j+1]==01);
            }else if(i==TAB_HAUT && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i-1][j]==10) + (board[i-1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i-1][j]==01) + (board[i-1][j-1]==01);
            }
        }
    }
    if(cornerB+cornerW != 0)
        heuristic += (colorPawn=='b')?
            (float)((cornerW-cornerB)*1500/(cornerB+cornerW)) : (float)((cornerB-cornerW)*1500/(cornerB+cornerW)) ;

    if(cntB+cntW != 0)
        heuristic -= (colorPawn=='b')?
            (float)((cntW-cntB)*500/(cntB+cntW)) : (float)((cntB-cntW)*500/(cntB+cntW));
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

//compute best move on one move
Coords* choseAGoodMove(int** board, char colorPawn){
    MatrixCoordinates* possibleMoveCoords;
    int value;
    possibleMoveCoords = possibleMove(board, colorPawn);
    Coords* bestCoordMove = (Coords*)malloc(sizeof(Coords));
    if((value=isPawnUntakable(board, possibleMoveCoords))!=64){
        bestCoordMove->x = possibleMoveCoords->matrix[value]->x;
        bestCoordMove->y = possibleMoveCoords->matrix[value]->y;
        return bestCoordMove;
    }
    if(possibleMoveCoords->numberOfCoord>0){
        int i,j,h, k, nbrCoords = possibleMoveCoords->numberOfCoord;
        int cntB=0, cntW=0;
        int bestScore = -INT_MAX;
        int boardSave[TAB_HAUT][TAB_LARG];
        for(k=0; k< TAB_HAUT; k++){
            for(j=0; j< TAB_LARG; j++)
                boardSave[k][j] = board[k][j];
        }
        for(i=0; i< nbrCoords; i++){
            // update board
            updateMatrix(board, colorPawn, possibleMoveCoords->matrix[i]);

            float pointCornerW=0, pointCornerB=0;
            int coefA=3500, coefB=0;
            int matricePoint[TAB_HAUT][TAB_LARG] =
                                 {  { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefA } ,
                                    { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                                    {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                                    {   0   ,   0   ,  0  , 0 , 0 , 0   ,   0   ,   0   } ,
                                    {   0   ,   0   ,  0  , 0 , 0 , 0   ,   0   ,   0   } ,
                                    {  500  ,  400  , 250 , 0 , 0 , 250 ,  400  ,  500  } ,
                                    { coefB , coefB , 400 , 0 , 0 , 400 , coefB , coefB } ,
                                    { coefA , coefB , 500 , 0 , 0 , 500 , coefB , coefB } };
            coefB= -2500;
             for(h=0; h<TAB_HAUT; h+=TAB_HAUT-1){
                for(j=0; j<TAB_LARG; j+=TAB_LARG-1){
                     if(h==0 && j==0 && board[h][j]==0){
                        matricePoint[h][j+1]=coefB;
                        matricePoint[h+1][j]=coefB;
                        matricePoint[h+1][j+1]=coefB;
                    }
                    else if(h==0 && j==TAB_LARG && board[h][j]==0){
                        matricePoint[h][j-1]=coefB;
                        matricePoint[h+1][j]=coefB;
                        matricePoint[h+1][j-1]=coefB;
                    }
                    else if(h==TAB_HAUT && j==0 && board[h][j]==0){
                        matricePoint[h][j+1]=coefB;
                        matricePoint[h-1][j]=coefB;
                        matricePoint[h-1][j+1]=coefB;
                    }
                    else if(h==TAB_HAUT && j==TAB_LARG && board[h][j]==0){
                        matricePoint[h][j-1]=coefB;
                        matricePoint[h-1][j]=coefB;
                        matricePoint[h-1][j-1]=coefB;
                    }
                }
            }
            //we compute the difference en posseded corners
            for(h=0; h<TAB_HAUT; h++){
                for(j=0; j<TAB_LARG; j++){
                    if(board[h][j] == 1)
                        pointCornerW += matricePoint[h][j];
                    else if(board[h][j] == 10)
                        pointCornerB += matricePoint[h][j];
                }
            }

            // compute the difference of pawn and put the board is as is was before
            for(k=0; k< TAB_HAUT; k++){
                for(j=0; j< TAB_LARG; j++){
                    if(board[k][j] == 1)
                        cntB++;
                    else if(board[k][j] == 10)
                        cntW++;
                    board[k][j] = boardSave[k][j];
                }
            }
            if (colorPawn == 'b' && (cntB-cntW)*1500+(pointCornerB-pointCornerW)>bestScore){
                bestScore = cntB-cntW*1500+(pointCornerB-pointCornerW);
                bestCoordMove->x = possibleMoveCoords->matrix[i]->x;
                bestCoordMove->y = possibleMoveCoords->matrix[i]->y;
            }else if(colorPawn == 'w' && (cntW-cntB)*1500+(pointCornerW-pointCornerB)>bestScore){
                bestScore = (cntW-cntB)*1500+(pointCornerW-pointCornerB);
                bestCoordMove->x = possibleMoveCoords->matrix[i]->x;
                bestCoordMove->y = possibleMoveCoords->matrix[i]->y;
            }
        }
        freeMatrixCoordinate(possibleMoveCoords);
        return bestCoordMove;
    }
    freeMatrixCoordinate(possibleMoveCoords);
    bestCoordMove->x = -1;
    bestCoordMove->y = -1;
    return bestCoordMove;
}

/*
int evaluateSituationMax(int** board, char colorPawn){
    int i, j, cntB=0, cntW=0, cornerW=0, cornerB=0, heuristic=0;
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
    // we compute the mobility
    int nbrMoov = numberPossibleMove(board, colorPawn);
    heuristic += (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) ));
///    printf("+ %d  = ", (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
///         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
///        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) )));

    cntB=0; cntW=0;
    //we compute the difference en posseded corners
    for(i=0; i<TAB_HAUT; i+=TAB_HAUT-1){
        for(j=0; j<TAB_LARG; j+=TAB_LARG-1){
            if(board[i][j] == 1)
                cornerB++;
            else if(board[i][j] == 10)
                cornerW++;// 01 06   10 11 16 17     60 61 66 67   71 76
            else if(i==0 && j==0){
                cntB = (board[i][j+1]==10) + (board[i+1][j]==10) + (board[i+1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i+1][j]==01) + (board[i+1][j+1]==01);
            }
            else if(i==0 && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i+1][j]==10) + (board[i+1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i+1][j]==01) + (board[i+1][j-1]==01);
            }
            else if(i==TAB_HAUT && j==0){
                cntB = (board[i][j+1]==10) + (board[i-1][j]==10) + (board[i-1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i-1][j]==01) + (board[i-1][j+1]==01);
            }else if(i==TAB_HAUT && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i-1][j]==10) + (board[i-1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i-1][j]==01) + (board[i-1][j-1]==01);
            }
        }
    }
    if(cornerB+cornerW != 0)
        heuristic += (colorPawn=='b')?
            (float)((cornerB-cornerW)*1500/(cornerB+cornerW)) : (float)((cornerW-cornerB)*1500/(cornerB+cornerW));
    if(cntB+cntW != 0)
        heuristic -= (colorPawn=='b')?
            (float)((cntB-cntW)*500/(cntB+cntW)) : (float)((cntW-cntB)*500/(cntB+cntW));
    return heuristic;
}

int evaluateSituationMin(int** board, char colorPawn){
    int i, j, cntB=0, cntW=0, cornerW=0, cornerB=0, heuristic=0;

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

    // we compute the mobility
    int nbrMoov = numberPossibleMove(board, colorPawn);
    heuristic += (int)((float)(1.25-(cntB+cntW) /(TAB_HAUT*TAB_LARG))*( (colorPawn=='b')?
         ((nbrMoov-predPossibleMove)*1000/(nbrMoov+predPossibleMove))
        : ((predPossibleMove-nbrMoov)*1000/(nbrMoov+predPossibleMove)) ));

    cntB=0; cntW=0;
///TODO: we need to add a negative coefficient for each cell near the corner if the corner is not  taken
    //we compute the difference en posseded corners
    for(i=0; i<TAB_HAUT; i+=TAB_HAUT-1){
        for(j=0; j<TAB_LARG; j+=TAB_LARG-1){
            if(board[i][j] == 1)
                cornerB++;
            else if(board[i][j] == 10)
                cornerW++;// 01 06   10 11 16 17     60 61 66 67   71 76
            else if(i==0 && j==0){
                cntB = (board[i][j+1]==10) + (board[i+1][j]==10) + (board[i+1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i+1][j]==01) + (board[i+1][j+1]==01);
            }
            else if(i==0 && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i+1][j]==10) + (board[i+1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i+1][j]==01) + (board[i+1][j-1]==01);
            }
            else if(i==TAB_HAUT && j==0){
                cntB = (board[i][j+1]==10) + (board[i-1][j]==10) + (board[i-1][j+1]==10);
                cntW = (board[i][j+1]==01) + (board[i-1][j]==01) + (board[i-1][j+1]==01);
            }else if(i==TAB_HAUT && j==TAB_LARG){
                cntB = (board[i][j-1]==10) + (board[i-1][j]==10) + (board[i-1][j-1]==10);
                cntW = (board[i][j-1]==01) + (board[i-1][j]==01) + (board[i-1][j-1]==01);
            }
        }
    }
    if(cornerB+cornerW != 0)
        heuristic += (colorPawn=='b')?
            (float)((cornerW-cornerB)*1500/(cornerB+cornerW)) : (float)((cornerB-cornerW)*1500/(cornerB+cornerW)) ;

    if(cntB+cntW != 0)
        heuristic -= (colorPawn=='b')?
            (float)((cntW-cntB)*500/(cntB+cntW)) : (float)((cntB-cntW)*500/(cntB+cntW));
    return heuristic;
}*/
