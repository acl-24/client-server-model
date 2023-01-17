//
// Created by Alex Liu on 2022-11-21.
//

#ifndef INC_379A3_SERVER_H
#define INC_379A3_SERVER_H

#include <iostream>
// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>

#include <sys/time.h>
//#define PORT 8080

using namespace std;

int server(int port_num);
double getTime();

#endif //INC_379A3_SERVER_H
