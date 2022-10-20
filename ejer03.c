#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int bandera = 0;

void myhandler_even(int a) { printf("signal even ...\n"); bandera = 1; }
void myhandler__odd(int a) { printf("signal  odd ...\n"); bandera = 0; }

void par(int n) {
    int pid = fork();
    int pares[n];
    pares[0] = 0;
    int i = 0;
    struct sigaction info, handler;
    handler.sa_handler = &myhandler__odd;

    for(i = 1; i < n; i++) pares[i] = pares[i-1] + 2;

    i = 0;
    while(1){
        if( bandera == 0 ) {
            printf("[ %d ] ",pares[i]);
            i++;
            kill(getppid(), SIGUSR1);
        }
        if( i == n) break;
    }
}

int main (){
    par(100);
}