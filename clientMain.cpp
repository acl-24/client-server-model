#include <iostream>
#include "client.h"

using namespace std;

int main(int argc, char *argv[]){
    if (strcmp(argv[0], "./client") == 0){
        int port_num = stoi(argv[1]);
        string ip_str = argv[2];
        if (port_num < 5000 || port_num > 64000){
            cerr << "Incorrect port number range" << endl;
            return 1;
        }
        client(port_num, ip_str);
        cout << port_num << endl;
        cout << ip_str << endl;
    }

    return 0;
}