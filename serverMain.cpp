#include <iostream>
#include "server.h"

using namespace std;

int main(int argc, char *argv[]){
    if (strcmp(argv[0], "./server") == 0){
        int port_num = stoi(argv[1]);
        if (port_num < 5000 || port_num > 64000){
            cerr << "Incorrect port number range" << endl;
            return 1;
        }
        server(port_num);
        cout << port_num << endl;
    }


    return 0;
}