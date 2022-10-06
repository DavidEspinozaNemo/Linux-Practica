#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 /* Windows XP. */
#endif
#include <Ws2tcpip.h>
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#if defined _WIN32
#define close(x) closesocket(x)
#endif
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];
    #if defined _WIN32
        WSADATA wsa_data;
        WSAStartup(MAKEWORD(1, 1), &wsa_data);
    #endif
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");
    //limpiar el mensage
    memset(server_reply,0,strlen(server_reply));
    // keep communicating with server
    while (1) {
        printf("Enter message : ");
        scanf("%s", message);
        // Send some data
        if (send(sock, message, strlen(message), 0) < 0) {
            puts("Send failed");
            return 1;
        }
        // Receive a reply from the server

        if (recv(sock, server_reply, 2000, 0) < 0) {
            puts("recv failed");
            break;
        }
        puts("Server reply :");
        puts(server_reply);
        //limpiar el mensage
        memset(server_reply,0,strlen(server_reply));
    }
    close(sock);
    #if defined _WIN32
        WSACleanup();
    #endif
    
    return 0;
}