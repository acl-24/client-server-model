//
// Created by Alex Liu on 2022-11-21.
//
#include "client.h"
#include "tands.h"
double getTime(){
    struct timeval Time;
    gettimeofday(&Time, nullptr);
    double seconds = (double) Time.tv_sec + (double) Time.tv_usec / 1000000;
    return seconds;
}


int client(int port_num, string ip){
    cout << "this is client " << port_num << " " << ip <<endl;

    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    FILE * output;

    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    string hostname_str = hostname;
    pid_t pid = getpid();

    string logname = hostname_str + "." + to_string(pid);
    output = fopen(logname.c_str(), "w+");
    fprintf(output, "Using Port %d\n", port_num);
    fprintf(output, "Using Server Address %s\n", ip.c_str());
    fprintf(output, "Host %s\n", logname.c_str());

    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    string input;
    string to_send = "";
    int n = 0;
    while (cin >> input){
        if (input[0] == 'T'){
            to_send += input;
            fprintf(output,"%6.3f: send (%s)\n", getTime(), input.c_str());
            n++;
        }
        if (input[0] == 'S'){
            int sleep_time = (int)input[1];
            fprintf(output,"Sleep %d units\n", sleep_time);
            Sleep(sleep_time);
        }
    }

    write(sock, to_send.c_str(), strlen(to_send.c_str()));
    
    string host_to_send = "@" + logname;
    write(sock, host_to_send.c_str(), strlen(host_to_send.c_str()));

    int m = n;
    while (m > 0){
        valread = read(sock, buffer, 1024);
        cout << buffer << endl;
        fprintf(output, "%6.3f: recv (%s)\n", getTime(), buffer);

        m--;
    }

    fprintf(output, "sent %d transactions\n", n);
    
    // closing the connected socket
    close(client_fd);

    return 0;
}
