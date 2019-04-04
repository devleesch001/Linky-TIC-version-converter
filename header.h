//
// Created by dev on 02/04/19.
//

#ifndef SERIAL___HEADER_H
#define SERIAL___HEADER_H

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>

#include <stdio.h>
#include <sys/types.h>  //création des sockets
#include <sys/socket.h> //création des sockets
#include <netinet/in.h> //contient les fonctions permettant de passer
#include <arpa/inet.h>  //du format local au format réseau htons....
#include <netdb.h>
#include <postgresql/libpq-fe.h>


using namespace std;

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"/*UART NAME IN PROCESSOR*/

#define STX 0x02
#define ETX 0x03
#define HT  0x09
#define LF  0x0a
#define CR  0x0d
#define SP  0x20

#define IP "192.168.2.105"   //IP du serveur à atteindre
#define PORT 3000

#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

void openport(void);
void readport(void);
int postgres(string adco, string east, string irms1, string urms1);
void analyse(string a);

#endif //SERIAL___HEADER_H
