// https://linuxhint.com/socket-connect-2-c-function/
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

char* ipv4 = "201.206.114.1"; //de las computadoras del tec

int main(int argc, char const* argv[]){
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hi = "Hi from client";
    char buffer[1024] = { 0 };

    if((sock = socket(AF_INEF, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if( inet_pton(AF_INET, ipv4, &serv_addr.sin_addr) <= 0){
        printf("\n Invalid address/ Address not supported \n");
        return -1;
    }

    if((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0){
        printf("\n Connection Failed \n");
        return -1;
    }

    send(sock, hi, strlen(hi), 0);
    printf("Hi message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
    close(client_fd);
    return 0;
}