#include "gameControler.h"

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

///Function to convert Binary to decimal
int binaryToDecimal(int binary)
{
    int power=0, decimal=0;
    do
    {
        decimal = decimal + pow(2, power)*(binary%2);
        binary/=10;
        power++;
    }
    while(binary!=0);
    return decimal;
}

///Function to convert decimal to hexadecimal
char* decimalToHexa(int decimal)
{
    int rest, sizeHexa=0,i;
    char temp[50] ="";
    char* hexa;

    do
    {
        rest = decimal%16;
        decimal = decimal/16;
        temp[sizeHexa] = ( ((rest<10) && (rest>=0)) ? '0' + rest : ('A'-10) + rest);
        sizeHexa++;
    }
    while(decimal != 0);
    hexa = (char*) malloc(sizeof(char)* sizeHexa);
    for(i=0; i<sizeHexa; i++)
    {
        hexa[i]= temp[sizeHexa-i-1];
    }
    hexa[sizeHexa]='\0';
    return hexa;
}

///Function to convert Hexa to decimal
int hexaToDecimal(char* hexa)
{
    int sizeHexa=strlen(hexa), power=0, decimal=0;
    for(power=0; power<sizeHexa; power++)
    {
        decimal = decimal + pow(16, power)*
                  ( ((hexa[(sizeHexa-1)-power]>='0')&&(hexa[(sizeHexa-1)-power]<='9')) ?
                    (int)hexa[(sizeHexa-1)-power]-'0' : (int)hexa[(sizeHexa-1)-power]-('A'-10) );
    }
    return decimal;
}

// convert Hexa into Binary
int hexaToBinary(char* hexa)
{
    return decimalToBinary(hexaToDecimal(hexa));
}
// convert Binary to Hexa
char* binaryToHexa(int binary)
{
    return decimalToHexa(binaryToDecimal(binary));
}

char cellIntToChar(int cell)
{
    switch (cell)
    {
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

int cellCharToInt(char cell)
{
    switch (cell)
    {
    case '_':
        return 0;
        break;
    case 'B':
        return 10;
        break;
    case 'W':
        return 01;
        break;
    default:
        return 00;
    }
    return 00;
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
    if(ch=='_' || ch=='X') color(FOREGROUND_RED ) ;/* | FOREGROUND_INTENSITY*/
    else if(ch=='B') color(FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
    else if(ch=='W') color(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY) ;
    //3. Affichage de l'item à l'écran
    printf("%c ", ch);

    //4. Retour de la couleur au blanc pour les autres affichages (contrat, nombre de coups restants)
    color(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED) ;
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

void display(char**tab)
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
            mettons_de_la_couleur(tab[i][j]);
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

char** fillTab(char** tab)
{
    //variables
    int i,j;

    for (i=0; i<TAB_HAUT; i++)
    {
        for (j=0; j<TAB_LARG; j++)
        {
            tab[i][j]='N';
            //printf("%c ",tab[i][j]);
        }
        //printf("\n");
    }

    return tab;
}

char** decodeBoard(int player,char serverLength[2000])
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
    //we add the new position of the player
    if (player) board[serverLength[4]-'0'][serverLength[3]-'0']='W';
    else board[serverLength[4]-'0'][serverLength[3]-'0']='B';

    return board;
}

void game()
{
    //variables
    int i;
    char** tab = malloc(TAB_HAUT*sizeof(char*));

    ///dynamic allocation
    for (i=0; i<TAB_HAUT; i++)
    {
        tab[i]=malloc(TAB_LARG*sizeof(char));
    }

    ///we fill the matrix
    tab=fillTab(tab);

    ///we display it
    display(tab);


    ///free the memory
    for (i=0; i<TAB_HAUT; i++)
        free(tab[i]);

    free(tab);
}

void gameLoop()
{
    char** board = (char**)malloc(sizeof(char*)*TAB_LARG);;
    int i,j;
    int socketGC,bytes,server_size,player=1,end=0;
    struct sockaddr_in server;
    char serverLength[2000];
    char *message;
    int err;
    char msgbuf [256];   // for a message up to 255 bytes.
    msgbuf [0] = '\0';    // Microsoft doesn't guarantee this on man page.
    //char server_reply[2000];

    for(i=0; i<TAB_HAUT; i++)
    {
        board[i] = (char*)malloc(sizeof(char)*TAB_LARG);
        for(j=0; j<TAB_LARG; j++)
        {
            board[i][j]='_';
        }
    }

    ///Create socket with
    //AF_INET=Ethernet socket
    //SOCK_STREAM=TCP socket (safe/slow)
    socketGC = socket(AF_INET, SOCK_STREAM, 0);

    //check the socket
    if (socketGC == -1)
    {
        printf("GC: Could not create socket");
    }
    puts("GC client socket created");

    ///Define the socket remote address : 127.0.0.1:8888
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");    //127.0.0.1=local address
    server.sin_port = htons(8888); //8888=port number

    //Set all bits of the padding field to 0 */
    memset(server.sin_zero, '\0', sizeof server.sin_zero);
    server_size=sizeof(struct sockaddr);

    ///Connect to remote server
    if (connect(socketGC, (struct sockaddr *)&server, (socklen_t*)&server) < 0)
    {
        perror("GC: connect failed. Error\n");
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
    puts("GC client socket connected");


    while(!end) //ajouter condition fin de jeu (grille remplie toussa)
    {
        ///Send msg to the server
        message=sendConnect("GameControler");
        send(socketGC,message,message[1]-'0',0);
        printf("GC sends CONNECT msg");

        ///Check if we receive a STATUS msg
        bytes=recv(socketGC,serverLength,2000,0);
        //printf("\nreceive %d bytes of data\n",bytes);

        if (serverLength[2]-'0'==6 || serverLength[2]-'0'==7)
        {
            //printf("\nCP: data received=%s\n",serverLength);
            printf("\nGC received STATUS msg\n");

            /// convertir le message en matrice de char using board
            //Convert board in boardInt (char to int) to display
            board=decodeBoard(player,serverLength);

            /*
            for(i=0; i<TAB_HAUT;i++){
                for(j=0; j<TAB_LARG;j++){
                    boardInt[i][j] = cellCharToInt(board[i][j]);
                }
            }
            */

            display(board);
            player=1-player;
        }

        else if (serverLength[2]-'0'==4) end=1;
    }
    for(i=0; i<TAB_HAUT; i++)
        free(board[i]);
    free(board[i]);

    Sleep(5000);

    //closing the socket
    close(socketGC);
}
