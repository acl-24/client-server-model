//
// Created by Alex Liu on 2022-11-21.
//

#ifndef INC_379A3_CLIENT_H
#define INC_379A3_CLIENT_H
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
#include <vector>

#include <sys/time.h>

using namespace std;

int client(int port_num, string ip);
double getTime();
#endif //INC_379A3_CLIENT_H
