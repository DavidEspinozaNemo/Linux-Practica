#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <regex.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

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
void search_regular_expression (char *file_name, char *regular_expression){
	
    //printf("%s \n", regular_expression);
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    char *s;
    regex_t regex;
    regmatch_t pmatch[1];
    regoff_t off, len_regex, off_total;

    char newfile_name[100] = "./";
    strcat(newfile_name, file_name);

    fp = fopen( newfile_name , "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    if (regcomp(&regex, regular_expression, REG_NEWLINE))
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%s", line);
        s = line;
        off_total = 0;
        //printf("Matches:\n");
        for (int i = 0; ; i++) {
            if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))
                break;

            off = pmatch[0].rm_so + (s - line);
            off_total += off;
            len_regex = pmatch[0].rm_eo - pmatch[0].rm_so;
            //printf("#%d:\n", i);
            //printf("offset = %jd; length = %jd\n", (intmax_t) off,(intmax_t) len_regex);
            //printf("substring = \"%.*s\"\n", len_regex, s + pmatch[0].rm_so);
            printf("similarity found in the document %s, line %jd; \n", file_name, (intmax_t) off_total );

            s += pmatch[0].rm_eo;
        }
    }

    fclose(fp);
    if (line)
        free(line);
    //exit(EXIT_SUCCESS);
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
        search_regular_expression( msg.namefile, regular_expression);
        	
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

    sendfilemessage("file-a.txt");
	/*sendfilemessage("archivo_b.txt");
	sendfilemessage("archivo_c.txt");
	sendfilemessage("archivo_d.txt");
	sendfilemessage("archivo_e.txt");*/
	//grep("lorem kcsdmc ...", N); -> tiempo ejecucion, imprimir documento y linea expresion regular
	// archivo_a, linea 14
	// ....
	// 45 milisigundos
	// N = 5
    getmessage("ipsum");

    return 0;
}
