#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

// definicion del mensage
#define MSGSZ 200
struct filemessage {
    long type;
    char namefile[MSGSZ];
}msg;

//aqui llevo la cuenta de la cantidad de mensages enviados
int totalmessage = 0;

//funcion para enviar un mensage
void sendfilemessage(char *mensagetxt){
    key_t msqkey = 999;
    int msqid = msgget(msqkey, IPC_CREAT | S_IRUSR | S_IWUSR);
    msg.type = 1;
    strcpy(msg.namefile, mensagetxt);
    msgsnd(msqid,(void *)&msg, sizeof(msg.namefile) ,IPC_NOWAIT);
    printf("Message sent\n");
    totalmessage++;
}

//funcion para buscar una exprecion regular
void search_regular_expression (){
    //abre el archivo filename
    //aplica regcom y regexec a cada linea del archivo

}

// funcion para recibir el mensage
void getmessage(){
    key_t msqkey = 999;
    int msqid = msgget(msqkey, IPC_CREAT | S_IRUSR | S_IWUSR);
    msg.type = 1;

    if( totalmessage <= 0 ){
        printf("No message \n");
    }
    else{
        msgrcv(msqid, &msg, MSGSZ, 1, 0);
        if ( msgrcv == NULL ) exit(0); //si o recibe el mensaje se va.
        printf("Message received, file  %s in process\n",msg.namefile);

        //search_regular_expression( filename, regular_expression)
        totalmessage--;
    }
    
}

// funcion grep;
/*1) Recibe la expresion regular.
  2) Crea, elimina y administra los hilos.
  3) cada hilo ejecuta getmessage*/

int main(){

    sendfilemessage("filea.txt");
    sendfilemessage("fileb.txt");
    sendfilemessage("filec.txt");
    getmessage();
    getmessage();
    getmessage();
    getmessage();
    sendfilemessage("filed.txt");
    getmessage();
    getmessage();

    return 0;
}
