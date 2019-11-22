#include <stdio.h>
#include <stdlib.h>
#include<string.h> //strlen
#include <winsock2.h> //to use sockets on Windows
//#include<sys/socket.h>
//#include<arpa/inet.h> //inet_addr
#include "clientPlayer.h"
#include <unistd.h> //write
#include <pthread.h>
#include <time.h> // aléatoire pour une des IA
#include "ArtificialIntelligence.h"

///type
typedef int socklen_t;
//pthread_mutex_t mutex;

///structure


///functions
void *pthreadCPFunc(void *arg)
{
    clientWP();
    return NULL;
}

void *pthreadGMFunc(void *arg)
{
    server2();
    return NULL;
}

///main
int main(int argc, char *argv[])
{
    //To use sockets on Windows
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData); //initialize the WinSock library

    //game();
    //Variables
    int ret;
    int n=5;
    pthread_t pthCP,pthGM;

    /**
********
  TEST
********
    */
    int i,j;
    int** board = (int**)malloc(sizeof(int*)*TAB_HAUT);
    for(i=0; i<TAB_HAUT; i++){
        board[i] = (int*)malloc(sizeof(int)*TAB_LARG);
        for(j=0; j<TAB_LARG; j++)
            board[i][j] = 0;
    }
    board[4][4]=10;
    board[3][3]=10;
    board[3][4]=01;
    board[4][3]=01;
    display(board);
    srand(time(NULL));

    /// example d'arbre
    int boucle=0;
    int win=0;
    for(boucle=0; boucle<10; boucle++){
        win+=test(board);
        printf("black win %d / %d\n",win, boucle+1);
        for(i=0; i<TAB_HAUT; i++){
            for(j=0; j<TAB_LARG; j++)
                board[i][j] = 0;
        }
        board[4][4]=10;
        board[3][3]=10;
        board[3][4]=01;
        board[4][3]=01;

        printf("Ready for Next match ?\n");
        system("PAUSE");
    }

    ///createJarvis(board);
    display(board);
    Sleep(1000);
    /**
*******
  END
*******
    */
    //mutex=PTHREAD_MUTEX_INITIALIZER;

    ///we create two threads
    ret=pthread_create(&pthCP,NULL,pthreadCPFunc,&n);
    //if there is a problem
    if (ret!=0) fprintf(stderr,"%s",strerror(ret)); // use fprintf

    ret=pthread_create(&pthGM,NULL,pthreadGMFunc,&n);
    //if there is a problem
    if (ret!=0) fprintf(stderr,"%s",strerror(ret)); // use fprintf

    ///we wait for the threads to finish
    ret=pthread_join(pthCP,NULL);
    //if there is a problem
    if (ret!=0) fprintf(stderr,"%s",strerror(ret)); // use fprintf

    ret=pthread_join(pthGM,NULL);
    //if there is a problem
    if (ret!=0) fprintf(stderr,"%s",strerror(ret)); // use fprintf

    puts("\nend of main\n");

    //We deallocate the ressources allocated by the function WSAStartup()
    WSACleanup();

    return 0;
}
