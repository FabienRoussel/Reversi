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

///STATUS1 msg
char* sendStatus1(int lastX, int lastY, int boardState[BOARD_SIZE])
{
    //variables
    int i=0, j=0;
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
        msg[i]=boardState[j]+'0'; //we copy the name
        j++;
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
/*int statusParser(int *msg, int WP)
{
    //variables
    int i=0, j=0, k=0,lastX,lastY,sizeX,sizeY;
    int length=msg[1];
    int type=msg[2];
    int nameLength;
    char *name;
    char **tab;

    //Handling synchro

    ///Handling msg type
    switch (type)
    {
    case 6: ///STATUS1
        lastX=msg[3];
        lastY=msg[4];
        sizeX=msg[5];
        sizeY=msg[6];

        k=7;

        tab = malloc(sizeY*sizeof(char*));

        //dynamic allocation
        for (i=0; i<sizeY; i++)
        {
            tab[i]=malloc(sizeX*sizeof(char));
        }

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
        int BP_pt, WP_pt, BP_timer, WP_timer, BP_nameLength, WP_nameLength;
        return 0;
        break;

    case 8: ///CONTROL
        perror("cannot receive a CONTROL msg");
        return 0;
        break;

    default:
        puts("incompatible type");
        return 0;
        break;
    }

}*/

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
    //display(tab);


    ///free the memory
    for (i=0; i<TAB_HAUT; i++)
    {
        free(tab[i]);
    }

    free(tab);
}
