#include "server.h"
#include "tands.h"

FILE * output;

double getTime(){
    struct timeval Time;
    gettimeofday(&Time, nullptr);
    double seconds = (double) Time.tv_sec + (double) Time.tv_usec / 1000000;
    return seconds;
}

int server(int port_num){
    string logname = "server.log";

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int status;
    

    output = fopen(logname.c_str(), "w+");
    fprintf(output, "Using Port %d\n", port_num);


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int block_status = fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port_num);

    // Forcefully attaching socket to the port
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    // if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    //     perror("bind failed");
    //     exit(EXIT_FAILURE);
    // }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    double start_time = getTime();
    double current_time;
    double all_trans_done = 0;
    double time_spent = 0;

    while (true){
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket == -1){
            if (errno == EWOULDBLOCK) {
                printf("No pending connections; sleeping for one second.\n");
                sleep(1);
                current_time = getTime();
                if ((current_time - start_time) >= 30){
                    fprintf(output, "%2.1f transactions/sec (%d / %1.3f)", all_trans_done/time_spent, (int)all_trans_done, time_spent);
                    exit(0);
                }
                continue;
            } else {
                perror("error when accepting connection");
                exit(1);
            }
        }

        if (new_socket >= 0){
            // socket_action(new_socket);
                    //local constant initialization
            queue<string> task_list;
            char buffer[1024] = { 0 };
            struct   timeval tv;
            tv.tv_sec = 1;

            fd_set   fdread;
            bool read_flag = false;

            while (!read_flag){
                FD_ZERO(&fdread);
                FD_SET( new_socket, &fdread );

                int select_status = select(FD_SETSIZE, &fdread, NULL, NULL, &tv) < 0;

                switch (select_status)
                {
                case 0:
                    break;

                case -1:
                    exit(-1);
                    break;
                
                default:
                    Sleep(99);
                    //read tasks from client
                    int read_n = read(new_socket, buffer, 1024);
                    start_time = getTime();
                    // start_time = getTime();
                    if (read_n == -1){
                        perror("Unable to read");
                        exit(-1);
                    }
                    read_flag = true;
                    break;
                }
            }

            char hostname[1024];
            strncpy(hostname, buffer, 1024);
            for (int i = 0; i < 1024; i ++){
                if (hostname[i] == '@') {
                    hostname[i] = ' ';
                    break;
                } else if (hostname[i] != (char)0){
                    hostname[i] = ' ';
                } else {
                    continue;
                }
            }
            
            for(int i = 0; i<1024; i++){
                if((isspace(hostname[i])) && (isspace(hostname[i+1])))
                    hostname[i]=hostname[i+1];
            }


            //create queue for task for this client
            string task_str = "";
            for (int i = 0; i < 1024; i ++){
                if (buffer[i] == 'T'){
                    if (i != 0){
                        task_list.push(task_str);
                        fprintf(output,"%6.3f: # %ld (%s)   from host %s\n", getTime(), task_list.size(), task_str.c_str(), hostname);
                    }
                    task_str = "T";
                } 
                else if (task_str != "" && buffer[i] == '@') {
                    task_list.push(task_str);
                    fprintf(output,"%6.3f: # %ld (%s)   from host %s\n", getTime(), task_list.size(), task_str.c_str(), hostname);
                    task_str = "";
                    break;
                } else if (buffer[i] != (char)0){
                    task_str += buffer[i];
                }
                else {
                    continue;
                }
            }


            int total_trans = task_list.size();

            while (task_list.size() > 0){
                string task_to_do = task_list.front();
                task_list.pop();
                if (task_to_do == ""){
                    continue;
                }
                int time_to_do = stoi(task_to_do.substr(1));
                string done_task = "D" + task_to_do.substr(1);
                Trans(time_to_do);
                fprintf(output,"%6.3f: # %ld (%s)   from host %s\n", getTime(), total_trans-task_list.size(), done_task.c_str(), hostname);
                write(new_socket, done_task.c_str(), strlen(done_task.c_str()));
            }
            time_spent += getTime() - start_time;
            all_trans_done += total_trans;
            // closing the connected socket      
            close(new_socket);

            fprintf(output, "\n");
            fprintf(output, "SUMMARY\n");
            fprintf(output, "%d transactions from %s\n", total_trans, hostname);

        }
    }

    



    return 0;
}

