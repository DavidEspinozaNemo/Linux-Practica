#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int bandera = 0;

void myhandler_even(int a) { printf("signal even ...\n"); bandera = 1; }
void myhandler__odd(int a) { printf("signal  odd ...\n"); bandera = 0; }

void impar(int n) {
    int pid = fork();
    int impares[n];
    impares[0] = 1;
    int i = 0;
    struct sigaction info, handler;
    handler.sa_handler = &myhandler_even;

    for(i = 1; i < n; i++) impares[i] = impares[i-1] + 2;

    i = 0;
    while(1){
        if( bandera == 1 ) {
            printf("[ %d ] ",impares[i]);
            i++;
            kill(getppid(), SIGUSR1);
        }
        if( i == n) break;
    }
}

int main() {
    impar(100);
}