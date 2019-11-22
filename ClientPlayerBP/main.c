#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <winsock2.h> //to use sockets on Windows
#include <unistd.h> //write
#include <time.h> // aléatoire pour une des IA
#include "ArtificialIntelligence.h"
#include "clientPlayer.h"

///type
typedef int socklen_t;

///main
int main(int argc, char *argv[])
{
  //To use sockets on Windows
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData); //initialize the WinSock library
 int ok;

    clientWP();

    //We deallocate the ressources allocated by the function WSAStartup()
    WSACleanup();

    Sleep(5000);
     scanf("%d",&ok);

    return 0;
}
