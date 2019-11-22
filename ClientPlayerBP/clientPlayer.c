#include "clientPlayer.h"

///Client template
//Create a socket
//Associate a socket with a local terminaison point
//Listen to a socket
//Accept a connection call with the client
//Close the connection socket


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


///Connect msg
char* sendConnect(char* name)
{
    //variables
    int i=0,j=0;
    int type=1;
    int nameLength=strlen(name);
    int checkSum=nameLength+2; //type+byte1+byte2+...+byteN
    int msgLength=checkSum+3; //msg content length
    char *msg = (char*) malloc(msgLength*sizeof(char));

    msg[0]='0';      //SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=nameLength+'0';
    msg[3]=nameLength+'0';

    for (i=4; i<4+nameLength; i++)
    {
        msg[i]=name[j]; //we copy the name
        j++;
    }
    msg[msgLength-1]=checkSum+'0';

    printf("msg length=%d \n",msgLength);

    return msg;
}

///New move msg
char* sendNewMove(int x, int y)
{
    //variables
    int type=3;
    int checkSum=3; //type+byte1+byte2+...+byteN
    int msgLength =checkSum+3; //total length

    char *msg = (char*)malloc(msgLength*sizeof(char));

    //msg[0]=       SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=x+'0';
    msg[4]=y+'0';
    msg[5]=checkSum+'0';

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

    //msg[0]=       SYNCHRO
    msg[1]=msgLength+'0';
    msg[2]=type+'0';
    msg[3]=0+'0';
    msg[4]=checkSum+'0';

    return msg;
}

///Message parser
int msgParserCP(int *msg, int WP)
{
    //variables
    //  int i=0,j=0,k=0,x,y,lastX,lastY,sizeX,sizeY;;
    //  int length=msg[1];
    int type=msg[2];
    //  int nameLength;
    //  char *name;
    //  char **tab;

    //Handling synchro
    //if (msg[0]==)

    ///Handling msg type
    switch (type)
    {
    case 2: ///OK or NOK
        if (msg[3]==0) return 2;//nok
        else if (msg[3]==1) return 1; //ok
        break;

    case 4: ///END
        return 4; ///the client player must disconnect and stop
        break;

    case 5: ///NEXT TURN
        return 5;
        break;

    default:
        puts("incompatible type");
        return 0;
        break;
    }
    return 0;
}

char** getBoard(char serverLength[2000])
{
    char** board = (char**)malloc(sizeof(char*)*TAB_HAUT);
    int i=0,j=0,k=0;

    //we fill the board with its content
    for (i=0; i<TAB_HAUT; i++)
    {
        board[i] = (char*)malloc(sizeof(char)*TAB_LARG);
        for (j=0; j<TAB_LARG; j++)
        {
            board[i][j]=serverLength[7+k];
            k++;
        }
    }

    return board;
}

void clientWP()
{
    int socketWP,bytes,x=0,y=0,server_size,coWP=0,end=0;
    struct sockaddr_in server;
    char serverLength[2000];
    char *message;
    int err;
    char    msgbuf [256];   // for a message up to 255 bytes.
    msgbuf [0] = '\0';    // Microsoft doesn't guarantee this on man page.
    //char server_reply[2000];

    /// for AI
    int i,j, depth=5, freeCells=0;
    Node* node;
    char c='b';
    char **matrice;
    int possibleMoveNbr;
    int **board = (int**)malloc(sizeof(int*)*TAB_HAUT);
    for(i=0; i<TAB_HAUT; i++)
    {
        board[i]=(int*)malloc(sizeof(int)*TAB_LARG);
        for(j=0; j<TAB_LARG; j++)
        {
            board[i][j]=0;
        }
    }

    ///Create socket with
    //AF_INET=Ethernet socket
    //SOCK_STREAM=TCP socket (safe/slow)
    socketWP = socket(AF_INET, SOCK_STREAM, 0);

    //check the socket
    if (socketWP == -1)
    {
        printf("CP: Could not create socket");
    }
    puts("BP client socket created");

    ///Define the socket remote address : 127.0.0.1:8888
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");    //127.0.0.1=local address
    server.sin_port = htons(8888); //8888=port number

    //Set all bits of the padding field to 0 */
    memset(server.sin_zero, '\0', sizeof server.sin_zero);
    server_size=sizeof(struct sockaddr);

    ///Connect to remote server
    if (connect(socketWP, (struct sockaddr *)&server, (socklen_t*)&server) < 0)
    {
        perror("CP: connect failed. Error\n");
        err = WSAGetLastError();
        FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
                       NULL,                // lpsource
                       err,                 // message id
                       MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT),    // languageid
                       msgbuf,              // output buffer
                       sizeof (msgbuf),     // size of msgbuf, bytes
                       NULL);               // va_list of arguments

        if (! *msgbuf) sprintf (msgbuf, "%d", err);  // provide error # if no string available
        printf("\nerrno= %s\n",msgbuf);
    }
    puts("BP client socket connected");

    ///Read and print the msg from the server into the buffer
    //recv(socketWP,server_reply,2000,0);

    //print the received msg
    //printf("CP: data received=%s",server_reply);

    if (!coWP)
    {
        ///Send msg to the server
        message=sendConnect("Morgane");
        send(socketWP,message,message[1]-'0',0);
        printf("BP sends CONNECT msg");

        ///Check if we receive a OK msg
        bytes=recv(socketWP,serverLength,2000,0);
        //printf("\nreceive %d bytes of data\n",bytes);

        if (serverLength[2]-'0'==2 && serverLength[3]-'0'==1)
        {
            //printf("\nCP: data received=%s\n",serverLength);
            printf("\nBP received OK msg\n");
        }

        coWP=1;
    }

    while(!end)
    {
        ///Check if we receive NEXT TURN msg
        printf("BP waiting for a msg...\n");
        bytes=recv(socketWP,serverLength,2000,0);
        //printf("\nreceive %d bytes of data\n",bytes);

        if (serverLength[2]-'0'==5)
        {
            //printf("\nCP: data received=%s\n",serverLength);
            printf("\nBP received NEXT TURN msg\n");

            //Get the board
            matrice=getBoard(serverLength);

            //find the best move
            freeCells=0;

            for(i=0; i<TAB_HAUT; i++)
            {
                for(j=0; j<TAB_LARG; j++)
                {
                    if(matrice[i][j]=='X')
                    {
                        board[i][j]=00;
                        freeCells++;
                    }
                    else  if(matrice[i][j]=='b' || matrice[i][j]=='B')
                        board[i][j]=01;
                    else  if(matrice[i][j]=='w' || matrice[i][j]=='W')
                        board[i][j]=10;
                    else
                    {
                        board[i][j]=00;
                        freeCells++;
                    }
                }
            }
            if (depth>freeCells)
                depth= freeCells;
            printf("BP sends NEW MOVE msg");

            possibleMoveNbr=numberPossibleMove(board, 'b');
            if(possibleMoveNbr>0)
            {
                node = (Node*)malloc(sizeof(Node));
                node->grade = -INT_MAX;
                MinMax(depth, node, board, c, -INT_MAX, INT_MAX);
                for(i=0; i<node->successors; i++)
                {
                    if (node->grade == node->nextNodes[i]->grade)
                    {
                        x = node->nextNodes[i]->myMove->x;
                        y = node->nextNodes[i]->myMove->y;
                        break;
                    }
                }
                //IA trouve la meilleure position x et y a jouer
                printf("%d, %d   ",  x, y );
                message=sendNewMove(x,y);
            }
            else
                message=sendNewMove(-1,-1);
            send(socketWP,message,message[1]-'0',0);


            /*printf("CP sends NOK msg");
            message=sendNOK();
            send(socketWP,message,message[1]-'0',0);*/

            ///Wait for OK or NOK msg
            bytes=recv(socketWP,serverLength,2000,0);

            if (serverLength[2]-'0'==2 && serverLength[3]-'0'==0)
            {
                printf("\nBP received NOK msg\n");
            }

            else if (serverLength[2]-'0'==2 && serverLength[3]-'0'==1) printf("\nBP received OK msg\n");

            else if (serverLength[2]-'0'==4)
            {
                printf("\nBP received END msg\n");
                end=1;
            }

            //printf("end of if\n");
        }

        else Sleep(750);
    }
    for(i=0; i<TAB_HAUT; i++)
        free(board[i]);

    free(board);


    Sleep(2000);

    //closing the socket
    close(socketWP);
}


