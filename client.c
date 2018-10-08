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

//CONNECTS CLIENT TO SERVER
int client_connect(char* server_ip, int port){

    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket < 0){
        printf("Error Creating Socket");
    }

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    int res = connect(client_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (res <0){
        printf("Error Connecting to Socket");
    }

    return client_socket;
}

//CLOSES SOCKET
void socket_close(int client_socket){

    close(client_socket);
}

//WRITES IN SOCKET
void socket_write(int client_socket, char* str){

    long pos = 0;
    while(pos < strlen(str) + 1){
        pos += write(client_socket, str + pos, strlen(str) + 1 - pos);
    }
}

//PROCESS CLIENT MESSAGE
void client_process(int client_socket){

    int str_len = 255;
    char str[str_len];

    while(1){

        printf("\n---> Enter string to Send: ");
        scanf("%s",str);
        socket_write(client_socket,str);

        int len = recv(client_socket, str, str_len, 0);
        if (len == 0){
            break;
        }

        printf("Received from server: %s", str);
    }

}


//MAIN ROUTINE
int main(int argc, char* agrv[]){
    int client_socket = client_connect("127.0.0.1", PORT);
    printf("Connected to server\n");
    client_process(client_socket);
    socket_close(client_socket);
    printf("Disconnected from server\n");
    return 0;
}