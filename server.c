//
// Created by Joao Beleza on 08-Oct-18.
//


//LIBRARIES
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 1234


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
void server_process(int client_socket){

    int str_len = 255;
    char str[str_len];

    while(1){

        int len = recv(client_socket, str, str_len, 0);
        printf("Received: %s - Lenght: %d\n", str, len);
        if (len == 5 && str[0] == "q"){
            break;
        }

        socket_write(client_socket, str);
    }

    socket_close(client_socket);
}


//SERVER WILL READ ALL SOCKETS CONNECTED
void server_loop(int server_socket){

    while(1){

        struct sockaddr_in cli_addr;
        socklen_t client_addr_len = sizeof(cli_addr);

        int new_socket = accept(server_socket, (struct sockaddr *) &cli_addr, &client_addr_len);
        if( new_socket == -1){
            break;
        }

        printf("Connected to Client\n");
        server_process(new_socket);
        printf("Disconnected from client\n");
    }
}


//MAIN ROUTINE
int main(int argc, char* argv[]){

    int server_socket = server_init("127.0.0.1", PORT);
    printf("Server has Started\n");
    server_loop(server_socket);


}