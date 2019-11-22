#include "gameMaster.h"

///Template for sockets

//Declaration of the serveur socket
//...

//Declaration of the client socket
//...

//Creation of the socket using socket(.., .., ..)

//If the socket is alright (!= -1)

//Configuration using sockaddr_in
//s_addr = .. (IP address)
//sin_family = .. (Protocol)
//sin_port = ..   (Port)

//Connection with the client using bind(.., .., ..)

//If the connection is established (>0)
//We begin to listen (server mode) using listen(.., ..)

//if the socket is alright
//we wait for the client to connect using accept(.., .., ..)

//We close both the client and server socket using closesocket(...)


CoordsTab* CoordsToCoordsTab(Coords* coords)
{
    CoordsTab* newCoords = (CoordsTab*)malloc(sizeof(CoordsTab));
    newCoords->x = coords->x +1 ;
    newCoords->y = coords->y + 'A' ;
    return newCoords;
}

Coords* CoordsTabToCoords(CoordsTab* coords)
{
    Coords* newCoords = (Coords*)malloc(sizeof(Coords));
    newCoords->x = coords->x -1 ;
    newCoords->y = (int) (coords->y - 'A') ;
    return newCoords;
}

void freeMatrixCoordinate(MatrixCoordinates* matrix)
{
    int i;
    for (i=0; i< matrix->numberOfCoord; i++)
    {
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    free(matrix);
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

/// @Definition:
///     encode the board in order to be able to send it
/// @Parameters :
///     the board of the game in int** with X cells
/// @Output :
///     encoded Board in int* with X/4 cells
/// @example : {{10,00}, {10,01}} -> {10001001}
int* encodeBoard(int boardState[TAB_LARG][TAB_HAUT])
{
    int lig, col, indexNewBoard=-1, byteCode=0;
    //the new board
    int* encodedBoard = (int*) malloc(sizeof(int)* (TAB_HAUT*TAB_LARG /4) );

    /// les codes en commentaires ci dessous seront a supprimer pour la version finale, c'est juste pour débugger
    /*for(lig=0; lig< TAB_LARG; lig++){
        for(col=0; col< TAB_HAUT; col++){
            printf("%3d", boardState[lig][col]);
        }
        printf("\n");
    }*/
    for(lig=0; lig< TAB_LARG; lig++)
    {
        for(col=0; col< TAB_HAUT; col++)
        {
            // we add the 2 bytes of the next cell of the board
            byteCode=byteCode*100 + boardState[lig][col];
            // we increment index and check the rest of his division by 3
            if( (++indexNewBoard)%4 == 3 )
            {
                encodedBoard[indexNewBoard/4] = byteCode; // (char)binaryToDecimal(byteCode); // if we want it in char directly
                byteCode=0;
            }
        }
    }
    /*printf("\n\n");
    for(lig=0; lig< (TAB_HAUT*TAB_LARG /4); lig++){
        printf("%8d\n", encodedBoard[lig]);
    }*/
    return encodedBoard;
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
                for(i=0; i<cptCellAdver ; i++)
                {
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

///Function to convert decimal to binary number
// max value of decimal is 2023
int decimalToBinary(int n)
{
    int remainder;
    int binary = 0, i = 1;
    do
    {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
    }
    while(n != 0);
    return binary;
}

///Function to get the first two numbers of a bigger number
int* divideBinaryIntoInt(int decimal)
{
    char str[10];
    itoa(decimal,str,10);
    int* nb = (int*) malloc(sizeof(int)*4);

    int temp=(str[strlen(str)-2])-'0';
    temp=temp*10+(str[strlen(str)-1]-'0');

    nb[3]=temp;

    temp=(str[strlen(str)-4])-'0';
    if (temp==0) nb[2]=str[strlen(str)-3]-'0';
    else
    {
        temp=temp*1000+(str[strlen(str)-3]-'0');
        nb[2]=temp/100;
    }

    temp=(str[strlen(str)-6])-'0';
    if (temp==0) nb[1]=str[strlen(str)-5]-'0';
    else
    {
        temp=temp*100000+(str[strlen(str)-5]-'0');
        nb[1]=temp/10000;
    }

    temp=(str[strlen(str)-8])-'0';
    if (temp==0) nb[0]=str[strlen(str)-7]-'0';
    else
    {
        temp=temp*10000000+(str[strlen(str)-7]-'0');
        nb[0]=temp/1000000;

    }

    //printf("%d = %d %d %d %d",decimal,nb[0],nb[1],nb[2],nb[3]);
    //printf("nb=%d",nb);
    //puts(&str[strlen(str)-2]);

    return nb;
}

///Set the socket parameters
struct sockaddr_in setParameters(int port)
{
    struct sockaddr_in sock;
    //Set the socket parameter and affect them to the socket descriptor
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = INADDR_ANY; //"sin_addr"=internet address
    sock.sin_port = htons(port); //port number

    //Set all bits of the padding field to 0 */
    memset(sock.sin_zero, '\0', sizeof sock.sin_zero);

    return sock;
}

///Bind the socket with the server
int binding(int sock, struct sockaddr_in server)
{
    if( bind(sock,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("GM: bind failed. Error");
        return 0;
    }
    else return 1;

}

///Check the player move
int moveChecker(int x, int y, int WP)
{
    return 1;
}

///OK msg
char* sendOK()
{
    //variables
    int type=2;
    int checkSum=2; //type+byte1+byte2+...+byteN
    int msgLength =checkSum+3; //total length

    char *msg = (char*) malloc(msgLength*sizeof(char));

    msg[0]='0';      //SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=1+'0';
    msg[4]=checkSum+'0';

    return msg;
}

///NOK msg
char* sendNOK()
{
    //variables
    int type=2;
    int checkSum=2; //type+byte1+byte2+...+byteN
    int msgLength =checkSum+3; //total length

    char *msg = (char*) malloc(msgLength*sizeof(char));

    msg[0]='0';      //SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=0+'0';
    msg[4]=checkSum+'0';

    return msg;
}

///END msg
char* sendEND()
{
    //variables
    int type=4;
    int checkSum=1; //type+byte1+byte2+...+byteN
    int msgLength =checkSum+3; //total length

    char *msg = (char*) malloc(msgLength*sizeof(char));

    //msg[0]=       SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=checkSum+'0';

    return msg;
}

///NEXT TURN msg
char* sendNextTurn(int lastX, int lastY, char boardState[TAB_HAUT][TAB_LARG])
{
    //variables
    int i=0, j=0,k=0;
    int type=5;
    int checkSum=TAB_HAUT*TAB_LARG+3; //type+byte1+byte2+...+byteN
    int msgLength =checkSum+3; //total length
    char *msg = (char*) malloc(msgLength*sizeof(char));

    //msg[0]=       SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=lastX+'0';
    msg[4]=lastY+'0';
    msg[5]=TAB_LARG+'0';
    msg[6]=TAB_HAUT+'0';

    k=7;

    for (i=0; i<TAB_HAUT; i++)
    {
        for (j=0; j<TAB_LARG; j++)
        {
            msg[k]=boardState[i][j];
            k++;
        }
    }

    msg[msgLength-1]=checkSum+'0';

    return msg;
}

///STATUS1 msg
char* sendStatus1(int lastX, int lastY, char boardState[TAB_HAUT][TAB_LARG])
{
    //variables
    int i=0, j=0,k=0;
    int type=5;
    int checkSum=BOARD_SIZE+3; //type+byte1+byte2+...+byteN
    int msgLength =checkSum+3; //total length
    char *msg = (char*) malloc(msgLength*sizeof(char));

    //msg[0]=       SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=lastX+'0';
    msg[4]=lastY+'0';
    msg[5]=TAB_LARG+'0';
    msg[6]=TAB_HAUT+'0';

    for (i=7; i<7+msgLength; i++)
    {
        msg[i]=boardState[k][j]; //we copy the name

        if (j>=TAB_LARG)
        {
            j=0;
            k++;
        }
    }

    msg[msgLength-1]=checkSum+'0';

    return msg;
}

///STATUS2 msg
char* sendStatus2(int ptBP, int timerBP,char *nameBP)
{
    //variables
    int i=0,type=7,j=0;
    int lengthBP=sizeof(nameBP)/sizeof(char);
    int checkSum=lengthBP+5; //type+byte1+byte2+...+byteN
    int msgLength =checkSum+3; //total length

    char *msg = (char*) malloc(msgLength*sizeof(char));

    msg[0]='0';       //SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=ptBP+'0';
    msg[4]=timerBP+'0';
    msg[5]=lengthBP+'0';

    for (i=6; i<6+lengthBP; i++)
    {
        msg[i]=nameBP[j]+'0'; //we copy the name
        j++;
    }

    msg[msgLength-1]=checkSum+'0';

    return msg;
}

///Message parser
int msgParserGM(int *msg, int WP)
{
    //variables
    int i=0,j=0,k=0,x,y,lastX,lastY,sizeX,sizeY,BP_pt,WP_pt, BP_timer, WP_timer, BP_nameLength,WP_nameLength,nameLength;
    int type=msg[2];
    char **tab;
    char *WP_name, *BP_name, *name;
    //Handling synchro
    //if (msg[0]==)

    ///Handling msg type
    switch (type)
    {
    case 1: ///CONNECT
        nameLength=msg[3];
        name = (char*)malloc(nameLength*sizeof(char));

        for (i=0; i<nameLength; i++)
            name[i]=(char)msg[4+i];

        //if it's WP sending a msg
        if (WP) sendStatus2(0,0,name);
        //if it's BP
        else if (WP==0) sendStatus2(0,0,name);

        else
        {
            perror("connect couldn't be achieved");
            return 0;
        }

        break;

    case 2: ///OK or NOK
        ///BUT THE GM SHOULDN'T RECEIVE THAT
        if (msg[3]==0) return 2;//nok
        else if (msg[3]==1) return 1; //ok
        break;

    case 3: ///NEW MOVE
        x=msg[3];
        y=msg[4];

        if (moveChecker(x,y,WP)) return 3;
        else
        {
            perror("move couldn't be computed");
            return 0;
        }
        break;

    case 4: ///END
        return 4; ///the client player must disconnect and stop
        break;

    case 5: ///NEXT TURN
        return 5;
        break;

    case 6: ///STATUS1
        lastX=msg[3];
        lastY=msg[4];
        sizeX=msg[5];
        sizeY=msg[6];
        k=7;

        tab = (char**) malloc(sizeY*sizeof(char*));

        //dynamic allocation
        for (i=0; i<sizeY; i++)
            tab[i]= (char*) malloc(sizeX*sizeof(char));

        for (i=0; i<sizeY; i++)
        {
            for (j=0; j<sizeX; j+=4)
            {
                int temp=decimalToBinary(msg[k]); //we get the binary number (ex: 10110011)
                int *part=divideBinaryIntoInt(temp); //we get the 4 bytes separated (ex: 10 11 00 11) but if it starts by 0 the 0 is omitted

                tab[i][j]=part[0];  //we get the first two digit (ex: 10)
                tab[i][j+1]=part[1];    //the second two digit (ex: 11)
                tab[i][j+2]=part[2];    //the third two digit (ex: 00)
                tab[i][j+3]=part[3];    //the fourth two digit (ex: 11)
                k++;
            }
        }

        break;

    case 7: ///STATUS2
        BP_pt=msg[3];
        BP_timer=msg[4];
        BP_nameLength=msg[5];
        BP_name = (char*) malloc(BP_nameLength*sizeof(char));

        for (i=0; i<BP_nameLength; i++)
            BP_name[i]=msg[6+i]+'0';

        WP_pt=msg[6+BP_nameLength];
        WP_timer=msg[7+BP_nameLength];
        WP_nameLength=msg[8+BP_nameLength];
        WP_name=malloc(WP_nameLength*sizeof(char));

        for (j=0; j<WP_nameLength; j++)
            WP_name[j]=msg[9+BP_nameLength+j];

        return 0;
        break;

    case 8: ///CONTROL
        perror("GM: cannot receive a CONTROL msg");
        return 0;
        break;

    default:
        puts("incompatible type");
        return 0;
        break;
    }
    return 0;
}

int IsAllCellsTaken(int** board)
{
    int i,j, counter=0;
    for(i=0; i<TAB_HAUT; i++)
    {
        for(j=0; j<TAB_HAUT; j++)
        {
            counter+= (board[i][j]!=0);
        }
    }
    return (counter==TAB_HAUT*TAB_LARG);
}


///Create the servers
// FD_ZERO=clear an fd_set
// FD_ISSET=check if a descriptor
// FD_SET=add a descriptor to an fd_set
// FD_CLR=remove a descriptor from an fd_set
void createServer()
{
    //Variables
    int bytes,socketWP,socketBP,socketGC,numsock=0,i,j,client[MAX_CLIENTS],peer_addr_size,max_sd,activity,lastMoveX=0,lastMoveY=0,end=0;
    int coWP=0, coBP=0;
    char clientLength[2000];
    char color='w';
    char board [TAB_HAUT][TAB_LARG];
    int **boardInt= (int**)malloc(sizeof(int*)*TAB_HAUT);
    Coords* coordsMove = (Coords*)malloc(sizeof(Coords));
    int playersHasNotMove=0;
    char *clientMsg;
    struct sockaddr_in structGM,peer_addr;      //this struct contains the length, family, port, address and a 8th zero field for the socket

    ///Initialization
    //set of socket descriptors
    fd_set fds;

    //board
    for (i=0; i<TAB_HAUT; i++)
    {
        boardInt[i]= (int*)malloc(sizeof(int)*TAB_LARG);
        for (j=0; j<TAB_LARG; j++)
        {
            board[i][j]='X';
            boardInt[i][j]=0;
        }
    }

    ///initialize board
    board[3][3]='W';
    boardInt[3][3]=01;
    board[3][4]='B';
    boardInt[3][4]=10;
    board[4][3]='B';
    boardInt[4][3]=10;
    board[4][4]='W';
    boardInt[4][4]=01;

    ///1- GM socket creation
    //Create socket with AF_INET=domain=protocol family used (here TCP/IP), SOCK_STREAM=type (SOCK_STREAM for TCP/IP), 0=protocol
    socketWP = socket(AF_INET, SOCK_STREAM, 0);

    if (socketWP == -1) printf("Could not create WP socket\n");
    puts("GM: socket WP created\n");

    /*socketBP = socket(AF_INET, SOCK_STREAM, 0);

    if (socketBP == -1) printf("Could not create BP socket\n");
    puts("GM: socket BP created\n");

    socketGC = socket(AF_INET, SOCK_STREAM, 0);

    if (socketGC == -1) printf("Could not create GC socket\n");
    puts("GM: socket GC created\n");*/

    //Set the socket parameters and affect them to the socket descriptor
    structGM=setParameters(8888);
    //structBP=setParameters(8889);
    //structGC=setParameters(8890);

    //Bind (connection with the client) = link a socket with its local address
    if (binding(socketWP,structGM)) puts("GM: bind done\n");
    else exit(0); // exit(0)

    /*if (binding(socketBP,structBP)) puts("GM: bind done\n");
    else exit(0); // exit(0)

    if (binding(socketGC,structGC)) puts("GM: bind done\n");
    else exit(0); // exit(0)*/

    ///2- Listen on ports
    if (listen(socketWP, MAX_CLIENTS)!=0)
        perror("error listening GM socket\n");
    else  puts("Waiting for incoming connections GM socket...\n");

    //clear the socket set
    FD_ZERO(&fds);

    //add master socket to set
    FD_SET(socketWP, &fds);
    max_sd=MAX_CLIENTS;
    //FD_SET(socketBP,&readfds);
    //FD_SET(socketGC, &readfds);

    //  max_sd = max(socketWP,socketBP);
    // max_sd = max(max_sd,socketGC);



    ///3- Accept the incoming connections
    peer_addr_size = sizeof(struct sockaddr);

    while(!end)
    {
        //initialization
        lastMoveX=0;
        lastMoveY=0;

        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        if((activity = select(max_sd+1, &fds, NULL, NULL, NULL) <0)) printf("select error");

        for (i=0 ; i<max_sd+1 ; i++)
        {
            if (FD_ISSET(i, &fds))
            {
                printf("caca1\n");
                if (i==socketWP)
                {
                    printf("caca2\n");
                    if(numsock<MAX_CLIENTS)
                    {
                        printf("caca3\n");
                        client[numsock]=accept(socketWP, (struct sockaddr *)&peer_addr,(socklen_t*)&peer_addr_size);
                        if (client[numsock] == -1) perror("Error accepting client connection");
                        FD_SET(client[numsock],&fds);
                        numsock++;
                        printf("\nAccepting client connection...\n");
                    }

                    else printf("Ran out of socket space.\n");
                }

                else
                {
                    ///------WHITE PLAYER
                    printf("\nWhite player\n");
                    color='w';

                    if (!coWP)
                    {
                        //send new connection greeting message
                        ///Read and print the msg from the client into the buffer
                        bytes=recv(client[i],clientLength,2000,0);

                        ///Checking if this is a CONNECT msg and send OK and NEXT TURN msg
                        if (clientLength[2]-'0'==1)
                        {
                            printf("\nGM received a CONNECT msg from %d\n",client[j]);

                            printf("\nGM sends OK msg\n");
                            clientMsg=sendOK();
                            send(client[i],clientMsg,clientMsg[1]-'0',0);

                            Sleep(200);
                        }

                        coWP=1;
                    }

                    printf("\nGM sends NEXT TURN msg\n");
                    clientMsg=sendNextTurn(0,0,board);
                    send(client[i],clientMsg,clientMsg[1]-'0',0);

                    ///Wait for new move msg
                    bytes=recv(client[i],clientLength,2000,0);

                    ///Check if this is a NEW MOVE msg
                    if (clientLength[2]-'0'==3)
                    {
                        printf("\nGM received NEW MOVE msg from %d\n",client[i]);
                        printf("\nnew x=%d new y=%d",clientLength[3]-'0',clientLength[4]-'0');
                        //Vérifier la nvelle position
                        coordsMove->x=clientLength[3]-'0';
                        coordsMove->y=clientLength[4]-'0';

                        lastMoveX=clientLength[3]-'0';
                        lastMoveY=clientLength[4]-'0';

                        ///if this is a good move, send OK msg
                        if(isMyMoveValid(boardInt, color, coordsMove))
                        {
                            for (i=0; i<TAB_HAUT; i++)
                            {
                                for (j=0; j<TAB_LARG; j++)
                                {
                                    if (boardInt[i][j]==0)
                                        board[i][j]='X';
                                    if (boardInt[i][j]==10)
                                        board[i][j]='B';
                                    if (boardInt[i][j]==01)
                                        board[i][j]='W';
                                }
                            }

                            printf("\nGM sends OK msg\n");
                            clientMsg=sendOK();
                            send(client[i],clientMsg,clientMsg[1]-'0',0);
                            playersHasNotMove=0;
                        }
                        ///else send NOK msg
                        else
                        {
                            if(numberPossibleMove(boardInt, color)==0)
                            {
                                playersHasNotMove++;
                            }
                            printf("\nGM sends NOK msg\n");
                            clientMsg=sendNOK();
                            send(client[i],clientMsg,clientMsg[1]-'0',0);
                        }
                    }

                    if (IsAllCellsTaken(boardInt) || playersHasNotMove>=2)
                    {
                        printf("\nGM sends END msg\n");
                        clientMsg=sendEND();
                        send(client[i],clientMsg,clientMsg[1]-'0',0);
                        end=1;
                    }

                    ///------GAME CONTROLER
                    printf("\nGame controler\n");
                    color='b';

                    ///Read and print the msg from the client into the buffer
                    bytes=recv(client[i],clientLength,2000,0);

                    ///Checking if this is a CONNECT msg
                    if (clientLength[2]-'0'==1)
                    {

                        /*if ((client[2] = accept(socketGC, (struct sockaddr *)&peer_addr,(socklen_t*)&peer_addr_size))<0)
                        {
                            perror("accept");
                            exit(EXIT_FAILURE);
                        }

                        //inform user of socket number - used in send and receive commands
                        printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));*/

                        //send new status message
                        printf("\nGM sends STATUS msg\n");
                        clientMsg=sendStatus1(lastMoveX,lastMoveY,board);
                        send(client[i],clientMsg,clientMsg[1]-'0',0);
                    }

                    if (end)
                    {
                        clientMsg=sendEND();
                        send(client[i],clientMsg,clientMsg[1]-'0',0);
                    }

                    Sleep(200);

                    ///---------BLACK PLAYER
                    printf("\nBlack player\n");

                    if (!coBP)
                    {
                        //send new connection greeting message
                        ///Read and print the msg from the client into the buffer
                        bytes=recv(client[i],clientLength,2000,0);

                        ///Checking if this is a CONNECT msg and send OK and NEXT TURN msg
                        if (clientLength[2]-'0'==1)
                        {
                            printf("\nGM received a CONNECT msg from %d\n",client[j]);

                            printf("\nGM sends OK msg\n");
                            clientMsg=sendOK();
                            send(client[i],clientMsg,clientMsg[1]-'0',0);

                            Sleep(200);
                        }

                        coBP=1;
                    }


                    printf("\nGM sends NEXT TURN msg\n");
                    clientMsg=sendNextTurn(0,0,board);
                    send(client[i],clientMsg,clientMsg[1]-'0',0);

                    ///Wait for new move msg
                    bytes=recv(client[i],clientLength,2000,0);

                    ///Check if this is a NEW MOVE msg
                    if (clientLength[2]-'0'==3)
                    {
                        //Vérifier la nvelle position
                        coordsMove->x=clientLength[3]-'0';
                        coordsMove->y=clientLength[4]-'0';

                        lastMoveX=clientLength[3]-'0';
                        lastMoveY=clientLength[4]-'0';

                        ///if this is a good move, send OK msg
                        if(isMyMoveValid(boardInt, color, coordsMove))
                        {
                            for (i=0; i<TAB_HAUT; i++)
                            {
                                for (j=0; j<TAB_LARG; j++)
                                {
                                    if (boardInt[i][j]==0)
                                        board[i][j]='X';
                                    if (boardInt[i][j]==10)
                                        board[i][j]='B';
                                    if (boardInt[i][j]==01)
                                        board[i][j]='W';
                                }
                            }
                            printf("\nGM sends OK msg\n");
                            clientMsg=sendOK();
                            send(client[i],clientMsg,clientMsg[1]-'0',0);
                            playersHasNotMove=0;

                            //send new status message
                            printf("\nGM sends STATUS msg\n");
                            clientMsg=sendStatus1(lastMoveX,lastMoveY,board);
                            send(client[i],clientMsg,clientMsg[1]-'0',0);

                            Sleep(200);
                        }
                        ///else send NOK msg
                        else
                        {
                            if(numberPossibleMove(boardInt, color)==0)
                            {
                                playersHasNotMove++;
                            }
                            printf("\nGM sends NOK msg\n");
                            clientMsg=sendNOK();
                            send(client[i],clientMsg,clientMsg[1]-'0',0);
                        }
                    }

                    //while(!IsAllCellsTaken(boardInt) && playersHasNotMove<2);

                    if (IsAllCellsTaken(boardInt) || playersHasNotMove>=2)
                    {
                        printf("\nGM sends END msg\n");
                        clientMsg=sendEND();
                        send(client[i],clientMsg,clientMsg[1]-'0',0);
                        end=1;
                    }
                }
            }

        }

/*
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(socketWP, &readfds))
        {
            if ((new_socket = accept(socketWP, (struct sockaddr *)&peer_addr,(socklen_t*)&peer_addr_size))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
            client[MAX_CLIENTS-1]=new_socket;


            for(j=0; j<MAX_CLIENTS; j++)
            {
                //if one of the socket of client[] is in readfds that means that sth was sent by the client
                if (FD_ISSET(client[j],&readfds))
                {
                    if (j==0)
                    {
                    }

                    else if(j==1)
                    {

                    }

                    else if(j==2)
                    {



                    }
                }
            }


        }


        /// a new client is connected or data has been received
        // if (FD_ISSET(socketWP, &readfds) && !end)
        //{
        // color='w';*/

        /* if ((client[0] = accept(socketWP, (struct sockaddr *)&peer_addr,(socklen_t*)&peer_addr_size))<0)
         {
             perror("accept");
             exit(EXIT_FAILURE);
         }

         //inform user of socket number - used in send and receive commands
         printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));


         //while(!IsAllCellsTaken(boardInt) && playersHasNotMove<2);

         if (IsAllCellsTaken(boardInt) || playersHasNotMove>=2)
         {
             printf("\nGM sends END msg\n");
             clientMsg=sendEND();
             send(client[0],clientMsg,clientMsg[1]-'0',0);
             end=1;
         }

         //  }*/

        /// a new client is connected or data has been received
        // if (FD_ISSET(socketGC, &readfds))
        // {



        // }

        /// a new client is connected or data has been received
        // if (FD_ISSET(socketBP, &readfds) && !end)
        //{
        /*  printf("dans BP");

          if ((client[1] = accept(socketBP, (struct sockaddr *)&peer_addr,(socklen_t*)&peer_addr_size))<0)
          {
              perror("accept");
              exit(EXIT_FAILURE);
          }

          //inform user of socket number - used in send and receive commands
          printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));

          //}*/
    }

    closesocket(socketWP);
    closesocket(socketBP);
    closesocket(socketGC);

    for(i=0; i<TAB_HAUT; i++)
        free(boardInt[i]);

    free(boardInt);

    Sleep(2000);
}

/*///Make a loop and a tab
for (i=0; i<MAX_CLIENTS; i++)
{
    if ((client[i]=accept(socketWP,(struct sockaddr *)&peer_addr,(socklen_t*)&peer_addr_size))<0)
        perror("error accepting connection\n");
    else puts("Accepting connection client...\n");

    printf("\nAddress connection=%d\n",client[i]);
}

///Read and print the msg from the client into the buffer
bytes=recv(client[0],clientLength,2000,0);

///Checking if this is a CONNECT msg and send OK and NEXT TURN msg
if (clientLength[2]-'0'==1)
{
    printf("\nGM received a CONNECT msg from %d\n",client[0]);

    printf("\nGM sends OK msg\n");
    clientMsg=sendOK();
    send(client[0],clientMsg,clientMsg[1]-'0',0);

    Sleep(200);
}

///Other socket
if (listen(socketBP, MAX_CLIENTS)!=0)
    perror("error listening GM socket\n");
else  puts("Waiting for incoming connections GM socket...\n");

peer_addr_size = sizeof(struct sockaddr);

if ((client[1]=accept(socketBP,(struct sockaddr *)&peer_addr,(socklen_t*)&peer_addr_size))<0)
    perror("error accepting connection\n");
else puts("Accepting connection client...\n");

printf("\nAddress connection=%d\n",client[1]);


///Read and print the msg from the server into the buffer
bytes=recv(client[1],clientLength,2000,0);
//printf("\nreceive %d bytes of data\n",bytes);
//serverWP.sin_addr.s_addr = inet_addr("127.0.0.2");
//printf("\nGM: data received=%s\n",clientLength);

///Checking if this is a CONNECT msg and send OK and NEXT TURN msg
if (clientLength[2]-'0'==1)
{
    printf("\nGM received a CONNECT msg from %d\n",client[1]);

    printf("\nGM sends OK msg\n");
    clientMsg=sendOK();
    send(client[1],clientMsg,clientMsg[1]-'0',0);

    Sleep(200);
}


//}*/
