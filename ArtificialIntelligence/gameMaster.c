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

///Functions for threads
/*void *pthreadCPFunc(void *arg)
{
    clientWP();
    return NULL;
}

void *pthreadGMFunc(int *socketWP)
{
    server(socketWP);
    return NULL;
}*/

///Set the socket parameters
struct sockaddr_in setParameters()
{
    struct sockaddr_in sock;
    //Set the socket parameter and affect them to the socket descriptor
    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = inet_addr("127.0.0.1"); //"sin_addr"=internet address
    sock.sin_port = htons( 8888 ); //port number

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

///Message parser
int msgParserGM(int *msg, int WP)
{
    //variables
    int i=0,j=0,k=0,x,y,sizeX,sizeY, BP_nameLength,WP_nameLength,nameLength;
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
        BP_nameLength=msg[5];
        BP_name = (char*) malloc(BP_nameLength*sizeof(char));

        for (i=0; i<BP_nameLength; i++)
            BP_name[i]=msg[6+i]+'0';

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

///Create the servers
void createServer()
{
    /*//To use sockets on Windows
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData); //initialize the WinSock library

    //Variables
    pthread_t pthWP,pthBP,pthGC;
    int ret,socketGM;
    struct sockaddr_un peer_addr;
    socklen_t peer_addr_size;
    struct sockaddr_in structGM,client;      //this struct contains the length, family, port, address and a 8th zero field for the socket

    ///1- WP socket creation
    //Create socket with AF_INET=domain=protocol family used (here TCP/IP), SOCK_STREAM=type (SOCK_STREAM for TCP/IP), 0=protocol
    socketGM = socket(AF_INET, SOCK_STREAM, 0);

    if (socketGM == -1) printf("Could not create GM socket\n");
    puts("GM: socket created");

    //Set the socket parameters and affect them to the socket descriptor
    structGM=setParameters();

    //Bind (connection with the client) = link a socket with a "sockaddr" struct
    if (binding(socketGM,structGM)) puts("GM: bind done");
    else exit(0); // exit(0)

    ///4- Listen for WP
    if (listen(socketGM, 3)!=0)
        perror("error listening WP socket");
    else  puts("Waiting for incoming connections WP socket...");

    ///5- Accept the connection
    peer_addr_size = sizeof(struct sockaddr_un);

    if (accept(socketGM,(struct sockaddr *) &peer_addr,&peer_addr_size) == -1)
        perror("error accepting connection");
    else puts("Accepting connection...");

    ///we create a thread
    ret=pthread_create(&pthWP,NULL,pthreadWPFunc,&socketGM);
    //if there is a problem
    if (ret!=0) fprintf(stderr,"%s",strerror(ret)); // use fprintf

}

void server(int socketWP)
{
    int socketBP, socketGC, client_sock, c,i,j;
    int bytes=0, boolMove=0,x,y,playerResponse,endOfGame=0;
    //unsigned long on=1;
    struct sockaddr_in serverWP, serverBP, serverGC, client;      //this struct contains the length, family, port, address and a 8th zero field for the socket
    char clientLength[2000];
    char buffer[50];
    char *clientMsg;
    char** tab = malloc(TAB_HAUT*sizeof(char*));

    ///dynamic allocation
    for (i=0; i<TAB_HAUT; i++)
    {
        tab[i]=malloc(TAB_LARG*sizeof(char));
    }

    ///we fill the matrix
    tab=fillTab(tab);

    ///Accept the incoming connection and affect it to the new descriptor "client_sock".
    // "client_sock" will be used to communicate with the new client, "server" can be used to wait another incoming connection.
     //Accept and incoming connection
    c = sizeof(struct sockaddr_in);

    if (client_sock = accept(socketWP, (struct sockaddr *)&client,(socklen_t*)&c) )

        if (client_sock < 0)
        {
            perror("GM: accept failed");
            return 1; // exit(0)
        }
    puts("GM: Connection WP socket accepted");

    ///Send msg to the socket of the incoming connection
    //send(client_sock,client_message,2000,0);

    ///Read and print the msg from the server into the buffer
    bytes=recv(client_sock,clientLength,2000,0);
    //printf("\nreceive %d bytes of data\n",bytes);
    //serverWP.sin_addr.s_addr = inet_addr("127.0.0.2");
    //printf("\nGM: data received=%s\n",clientLength);

    ///Checking if this is a CONNECT msg and send OK and NEXT TURN msg
    if (clientLength[2]-'0'==1)
    {
        printf("\nGM received a CONNECT msg\n");

        printf("\nGM sends OK msg\n");
        clientMsg=sendOK();
        send(client_sock,clientMsg,clientMsg[1]-'0',0);

        Sleep(200);

        do
        {
            ///Send NEXT TURN msg
            printf("\nGM sends NEXT TURN msg to CP\n");
            clientMsg=sendNextTurn(0,0,tab);
            send(client_sock,clientMsg,clientMsg[1]-'0',0);
            //start timer

            ///Send STATUS1 msg to GC
            printf("\nGM sends NEXT TURN msg to GC\n");
            clientMsg=sendNextTurn(0,0,tab);
            send(client_sock,clientMsg,clientMsg[1]-'0',0);

            ///Wait for NEW MOVE msg
            bytes=recv(client_sock,clientLength,2000,0);
            playerResponse=clientLength[2]-'0';

            if (playerResponse==3)
            {
                //check the player move
                //get boolMove=1 if correct move, 0 if not valid
                printf("\nGM checks the player move...\n");
                x=clientLength[3]-'0';
                y=clientLength[4]-'0';
                boolMove=moveChecker(x,y,client_sock);

                //compute new board

                //if it's the END of the game
               // if () endOfGame=1;

                //stop timer

                ///send OK or NOK depending on boolMove value
                if (boolMove)
                {
                    printf("\nGM sends OK msg\n");
                    clientMsg=sendOK();
                    send(client_sock,clientMsg,clientMsg[1]-'0',0);
                }

                else
                {
                    printf("\nGM sends NOK msg\n");
                    clientMsg=sendNOK();
                    send(client_sock,clientMsg,clientMsg[1]-'0',0);
                }

                Sleep(200);
            }

            ///if the client send NOK instead of NEW MOVE
            else if (playerResponse==2)
            {
                ///we send OK msg
                printf("\nGM sends OK msg\n");
                clientMsg=sendOK();
                send(client_sock,clientMsg,clientMsg[1]-'0',0);

                ///we send END msg
                printf("\nGM sends END msg\n");
                clientMsg=sendEND();
                send(client_sock,clientMsg,clientMsg[1]-'0',0);

                endOfGame=1;

            }

        }
        while(!endOfGame);


    }


    //printf("\nGM sending: %s\n",clientMsg);

    //clientMsg=malloc((clientLength[1]-'0')*sizeof(char));
    // printf("\nclientLength[1]-'0'=%d strlen(clientMsg)=%d\n",clientLength[1]-'0',strlen(clientMsg));

     do
     {
         bytes=recv(client_sock,buffer,50,0);
          printf("\nreceive %d bytes of data\n",bytes);

          bytes+=bytes;
          printf("\nbytes=%d strlen(clientMsg)=%d\n",bytes,clientLength[1]-'0');

     }while(bytes<(clientLength[1]-'0'));*/

/*
    ///Close the connection
    //closesocket(socket_desc);
    closesocket(client_sock);
    puts("Close the connection");*/

}




void server2()
{
    int socketWP, socketBP, socketGC, client_sock, c,i;
    int bytes=0, boolMove=0,x,y,playerResponse,endOfGame=0;
    //unsigned long on=1;
    struct sockaddr_in serverWP, serverBP, serverGC, client;      //this struct contains the length, family, port, address and a 8th zero field for the socket
    char clientLength[2000];
    char *clientMsg;
    char** tab = malloc(TAB_HAUT*sizeof(char*));

    ///dynamic allocation
    for (i=0; i<TAB_HAUT; i++)
    {
        tab[i]=malloc(TAB_LARG*sizeof(char));
    }

    ///we fill the matrix
    tab=fillTab(tab);

    ///1- WP socket creation
    //Create socket with AF_INET=domain=protocol family used (here TCP/IP), SOCK_STREAM=type (SOCK_STREAM for TCP/IP), 0=protocol
    socketWP = socket(AF_INET, SOCK_STREAM, 0);

    if (socketWP == -1) printf("Could not create WP socket");
    puts("GM: WP socket created");

    //Set the socket parameters and affect them to the socket descriptor
    serverWP=setParameters();

    //Bind (connection with the client) = link a socket with a "sockaddr" struct
    if (binding(socketWP,serverWP)) puts("serverWP: bind done");
    else exit(0); // exit(0)

    ///2- BP socket creation
    socketBP = socket(AF_INET, SOCK_STREAM, 0);

    if (socketBP == -1) printf("Could not create BP socket");
    puts("GM: BP socket created");

    //Set the socket parameters
    serverBP=setParameters();

    //Bind
    //if (binding(socketBP,serverBP)) puts("serverWP: bind done");
    //else return 1;

    ///3- GC socket creation
    socketGC = socket(AF_INET, SOCK_STREAM, 0);

    //if there is an error while creating the socket
    if (socketGC == -1) printf("Could not create GC socket");
    puts("GM: GC socket created");

    //Set the socket parameters
    serverGC=setParameters();

    //Bind
    //if (binding(socketGC,serverGC)) puts("serverWP: bind done");
    //else return 1;

    //Wait for incoming sockets (clients), 3 sockets can be opened simultaneously
    ///4- Listen will block until a client incoming connection occurs
    if (listen(socketWP, 3)!=0)
        perror("error listening WP socket");
    else  puts("Waiting for incoming connections WP socket...");

    /*if (listen(socketBP,3)!=0)
        perror("error listening BP socket");
    else  puts("Waiting for incoming connections BP socket...");

    if (listen(socketGC,3)!=0)
        perror("error listening GC socket");
    else  puts("Waiting for incoming connections GC socket...");*/

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);

    ///Accept the incoming connection and affect it to the new descriptor "client_sock".
    // "client_sock" will be used to communicate with the new client, "server" can be used to wait another incoming connection.
    if ((client_sock = accept(socketWP, (struct sockaddr *)&client,(socklen_t*)&c)) )

        if (client_sock < 0)
        {
            perror("GM: accept failed");
             exit(0);
        }
    puts("GM: Connection WP socket accepted");

    ///Send msg to the socket of the incoming connection
    //send(client_sock,client_message,2000,0);

    ///Read and print the msg from the server into the buffer
    bytes=recv(client_sock,clientLength,2000,0);
    //printf("\nreceive %d bytes of data\n",bytes);
    //serverWP.sin_addr.s_addr = inet_addr("127.0.0.2");
    //printf("\nGM: data received=%s\n",clientLength);

    ///Checking if this is a CONNECT msg and send OK and NEXT TURN msg
    if (clientLength[2]-'0'==1)
    {
        printf("\nGM received a CONNECT msg\n");

        printf("\nGM sends OK msg\n");
        clientMsg=sendOK();
        send(client_sock,clientMsg,clientMsg[1]-'0',0);

        Sleep(200);

        do
        {
            ///Send NEXT TURN msg
            printf("\nGM sends NEXT TURN msg to CP\n");
            clientMsg=sendNextTurn(0,0,tab);
            send(client_sock,clientMsg,clientMsg[1]-'0',0);
            //start timer

            ///Send STATUS1 msg to GC
            printf("\nGM sends NEXT TURN msg to GC\n");
            clientMsg=sendNextTurn(0,0,tab);
            send(client_sock,clientMsg,clientMsg[1]-'0',0);

            ///Wait for NEW MOVE msg
            bytes=recv(client_sock,clientLength,2000,0);
            playerResponse=clientLength[2]-'0';

            if (playerResponse==3)
            {
                //check the player move
                //get boolMove=1 if correct move, 0 if not valid
                printf("\nGM checks the player move...\n");
                x=clientLength[3]-'0';
                y=clientLength[4]-'0';
                boolMove=moveChecker(x,y,client_sock);

                //compute new board

                //if it's the END of the game
               // if () endOfGame=1;

                //stop timer

                ///send OK or NOK depending on boolMove value
                if (boolMove)
                {
                    printf("\nGM sends OK msg\n");
                    clientMsg=sendOK();
                    send(client_sock,clientMsg,clientMsg[1]-'0',0);
                }

                else
                {
                    printf("\nGM sends NOK msg\n");
                    clientMsg=sendNOK();
                    send(client_sock,clientMsg,clientMsg[1]-'0',0);
                }

                Sleep(200);
            }

            ///if the client send NOK instead of NEW MOVE
            else if (playerResponse==2)
            {
                ///we send OK msg
                printf("\nGM sends OK msg\n");
                clientMsg=sendOK();
                send(client_sock,clientMsg,clientMsg[1]-'0',0);

                ///we send END msg
                printf("\nGM sends END msg\n");
                clientMsg=sendEND();
                send(client_sock,clientMsg,clientMsg[1]-'0',0);

                endOfGame=1;

            }

        }
        while(!endOfGame);


    }


    //printf("\nGM sending: %s\n",clientMsg);

    //clientMsg=malloc((clientLength[1]-'0')*sizeof(char));
    // printf("\nclientLength[1]-'0'=%d strlen(clientMsg)=%d\n",clientLength[1]-'0',strlen(clientMsg));

    /* do
     {
         bytes=recv(client_sock,buffer,50,0);
          printf("\nreceive %d bytes of data\n",bytes);

          bytes+=bytes;
          printf("\nbytes=%d strlen(clientMsg)=%d\n",bytes,clientLength[1]-'0');

     }while(bytes<(clientLength[1]-'0'));*/


    ///Close the connection
    //closesocket(socket_desc);
    closesocket(client_sock);
    puts("Close the connection");

}

