//
// Created by Joao Beleza on 08-Oct-18.
//


//LIBRARIES
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

#define PORT 1235


//INITIALIZE SERVER
int server_init(char* server_ip, int port){

    int server_socket = socket(PF_INET, SOCK_STREAM, 0);

    if (server_socket < 0){
        printf("Error Creating Socket");
    }

    int val = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (res < 0){
        printf("Error Binding Socket\n");
    }

    listen(server_socket, 5);
    return server_socket;
}

//CLOSES SOCKET
void socket_close(int client_socket){

    close(client_socket);
}

//WRITE IN SOCKET
void socket_write(int client_socket, char* str){

    long pos = 0;
    while( pos < strlen(str) + 1){
        pos += write(client_socket, str + pos, strlen(str) + 1 - pos);
    }
}

//SERVER WILL PROESS DATA
void server_process(int *client_sock){

    int str_len = 255;
    char str[str_len];
    int client_socket = *(int*) client_sock;

    while(1){

        int len = recv(client_socket, str, str_len, 0);
        printf("Received: %s - Lenght: %d\n", str, len);

        if (len == 2 && str[0] == 'q'){
            printf("Client Disconnected\n");
            fflush(stdout);
            break;
        }

        socket_write(client_socket, str);
    }

    socket_close(client_socket);
}


//SERVER WILL READ ALL SOCKETS CONNECTED
void server_loop(int server_socket){

    int client_socket,*new_socket;
    struct sockaddr_in cli_addr;
    socklen_t client_addr_len = sizeof(cli_addr);

    while(client_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &client_addr_len)){

        printf("Connection Accepted\n");

        pthread_t sniffer_thread;
        new_socket = malloc(1);
        *new_socket = client_socket;

        if(pthread_create(&sniffer_thread, NULL, server_process, (void*) new_socket) < 0){

            printf("Error creating Thread");
            return;
        }

    }

    if( client_socket < 0){
        printf("Error Connecting to Socket");
        return;
    }

    printf("Disconnected from client\n");
    return;
}


//MAIN ROUTINE
int main(int argc, char* argv[]){

    int server_socket = server_init("127.0.0.1", PORT);
    printf("Server has Started\n");
    server_loop(server_socket);

}