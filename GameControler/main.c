#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <winsock2.h> //to use sockets on Windows
#include <unistd.h> //write
#include <time.h> // aléatoire pour une des IA
#include "gameControler.h"

///type
typedef int socklen_t;

///main
int main(int argc, char *argv[])
{
  //To use sockets on Windows
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData); //initialize the WinSock library

    gameLoop();

    //We deallocate the ressources allocated by the function WSAStartup()
    WSACleanup();

    return 0;
}
