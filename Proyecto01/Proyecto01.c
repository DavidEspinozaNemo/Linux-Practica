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
#define DUCTSIZE 1000
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
void search_regular_expression (char *regular_expression){
    //abre el archivo filename
    //aplica regcom y regexec a cada linea del archivo
	FILE *fptr;
	char sentence[DUCTSIZE];
		
	//Abre el archivo llamado msg.namefile
	fptr = open(msg.namefile, "r");
		
	if( fptr != NULL ){ //si pudo abrir el archivo
		
		//lectura linea x linea del archivo	
			
		while(sentence != EOF){
				
			fgets(sentence, DUCTSIZE, fptr);
			printf("%s \n", sentence);
		};
			
		fclose(fptr); //cerrando el archivo
	} //si no pudo abrir el archivo
	else{
		printf("file %s can't be opened \n", msg.namefile);
	}
}

// funcion para recibir el mensage
void getmessage(char *regular_expression){
    key_t msqkey = 999;
    int msqid = msgget(msqkey, IPC_CREAT | S_IRUSR | S_IWUSR);
    msg.type = 1;

    if( totalmessage > 0 ){
        msgrcv(msqid, &msg, MSGSZ, 1, 0);
        if ( msgrcv == NULL ) exit(0); //si o recibe el mensaje se va.
        printf("Message received, file %s in process\n",msg.namefile);

		totalmessage--;
        //search_regular_expression( filename, regular_expression)
        	
    }
    else{
		printf("No message \n");
    }
}

// funcion grep;
/*1) Recibe la expresion regular.
  2) Crea, elimina y administra los hilos.
  3) cada hilo ejecuta getmessage*/

int main(){

    sendfilemessage("archivo_a.txt");
	/*sendfilemessage("archivo_b.txt");
	sendfilemessage("archivo_c.txt");
	sendfilemessage("archivo_d.txt");
	sendfilemessage("archivo_e.txt");*/
	//grep("lorem kcsdmc ...", N); -> tiempo ejecucion, imprimir documento y linea expresion regular
	// archivo_a, linea 14
	// ....
	// 45 milisigundos
	// N = 5
    getmessage("expresion regular");

    return 0;
}