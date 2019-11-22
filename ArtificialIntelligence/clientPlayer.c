#include "clientPlayer.h"

///Client template
//Create a socket
//Associate a socket with a local terminaison point
//Listen to a socket
//Accept a connection call with the client
//Close the connection socket

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

void clientWP()
{
    int socketWP,typeMsg=0,x=0,y=0;
    struct sockaddr_in server;
    char serverLength[2000];
    char *message;
    //char server_reply[2000];

    ///Create socket with
    //AF_INET=Ethernet socket
    //SOCK_STREAM=TCP socket (safe/slow)
    socketWP = socket(AF_INET, SOCK_STREAM, 0);

    //check the socket
    if (socketWP == -1)
    {
        printf("CP: Could not create socket");
    }
    puts("WP client socket created");

    ///Define the socket remote address : 127.0.0.1:8888
    server.sin_addr.s_addr = inet_addr("127.0.0.1");    //127.0.0.1=local address
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 ); //8888=port number

    //Set all bits of the padding field to 0 */
    memset(server.sin_zero, '\0', sizeof server.sin_zero);

    ///Connect to remote server
    if (connect(socketWP, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("CP: connect failed. Error");
    }
    puts("WP client socket connected");

    ///Read and print the msg from the server into the buffer
    //recv(socketWP,server_reply,2000,0);

    //print the received msg
    //printf("CP: data received=%s",server_reply);

    ///Send msg to the server
    printf("CP sends CONNECT msg");
    message=sendConnect("Morgane");
    send(socketWP,message,message[1]-'0',0);

    ///Check if we receive a OK msg
  ///  bytes=recv(socketWP,serverLength,2000,0);
    //printf("\nreceive %d bytes of data\n",bytes);

    if (serverLength[2]-'0'==2 && serverLength[3]-'0'==1)
    {
        //printf("\nCP: data received=%s\n",serverLength);
        printf("\nCP received OK msg\n");
    }

    do
    {
        ///Check if we receive NEXT TURN msg
        printf("CP waiting for a msg...\n");
///        bytes=recv(socketWP,serverLength,2000,0);
        //printf("\nreceive %d bytes of data\n",bytes);

        if (serverLength[2]-'0'==5)
        {
            //printf("\nCP: data received=%s\n",serverLength);
            printf("\nCP received NEXT TURN msg\n");

            //find the best move

            ///Send NEW MOVE or NOK msg
            printf("CP sends NEW MOVE msg");
            message=sendNewMove(x,y);
            send(socketWP,message,message[1]-'0',0);

            /*printf("CP sends NOK msg");
            message=sendNOK();
            send(socketWP,message,message[1]-'0',0);*/

            ///Wait for OK or NOK msg
///            bytes=recv(socketWP,serverLength,2000,0);

            if (serverLength[2]-'0'==2 && serverLength[3]-'0'==0)
            {
                printf("\nCP received NOK msg\n");
                typeMsg=4;
            }

            else if (serverLength[2]-'0'==2 && serverLength[3]-'0'==1) printf("\nCP received OK msg\n");

            else if (serverLength[2]-'0'==4)
            {
                printf("\nCP received END msg\n");
                typeMsg=4;
            }

            //printf("end of if\n");
        }


    }
    while(typeMsg!=4);

    //closing the socket
    close(socketWP);
}

//keep communicating with server
/*while(1)
{
    //printf("CP: Enter message: ");
    //scanf("%s", message);

    //Send some data
    //sock=socket used to write data
    //message=buffer containing data to send
    //strlen(message)=number of bytes to send

    //pthread_mutex_lock(&lock);

    if( write(sockfd, message, strlen(message)) < 0)
    {
        perror("CP: write failed");
             return 1;
    }
     //pthread_mutex_unlock(&lock);

    //Receive a reply from the server
    //sock=socket used to read data
    //server_reply=buffer used to put data coming from socket
    //10=number of bytes to read from the socket
    ///read will block until all bytes have been read
    if( read(sockfd , server_reply , 2000 ) < 0)
    {
        puts("CP: read failed");
        break;
    }
    puts("CP: Server reply :");

    puts(server_reply);
}

//Destroy the mutex
 pthread_mutex_destroy(&lock);*/
