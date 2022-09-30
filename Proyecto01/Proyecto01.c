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
#include <pthread.h>
 #include <time.h>

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

//mutex
pthread_mutex_t mutex;

//para medir el tiempo de ejecución
clock_t start, end;
double cpu_time_used;

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
	
	
	//posibles errores
	//if (fp == NULL) printf(" Error open %s \n", file_name);
	
	//if (regcomp(&regex, regular_expression, REG_NEWLINE)) printf(" Error create regex for %s \n", file_name);

    if ( (fp != NULL) && !(regcomp(&regex, regular_expression, REG_NEWLINE)) ) {
		
		while ((read = getline(&line, &len, fp)) != -1) {
			s = line;
			off_total = 0;
			//printf(" line: %s \n", s);
			for (int i = 0; ; i++) {
				if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))
					break;

				off = pmatch[0].rm_so + (s - line);
				off_total += off;
				len_regex = pmatch[0].rm_eo - pmatch[0].rm_so;
				printf("similarity found in the document %s, line %jd; \n", file_name, (intmax_t) off_total );

				s += pmatch[0].rm_eo;
			}
		}
    }
	
	fclose(fp);
	if (line)
		free(line);
}

// funcion para recibir el mensage
void* getmessage(void* arg){
    key_t msqkey = 999;
    int msqid = msgget(msqkey, IPC_CREAT | S_IRUSR | S_IWUSR);
    msg.type = 1;
	char regular_expression[1000];
	memcpy(regular_expression, arg, strlen(arg)); //copio el argumento en la variable local.

	pthread_mutex_lock(&mutex);
    if( totalmessage > 0 ){
        msgrcv(msqid, &msg, MSGSZ, 1, 0);
        if ( msgrcv != NULL ) { //si recibe el mensaje
			printf("Message received, file %s in process\n",msg.namefile);

			totalmessage--;
			search_regular_expression( msg.namefile, regular_expression);
		}
		else{ //si no lo resive
			printf("Message error \n");
		} 	
    }
    else{
		printf("No message \n");
    }
	pthread_mutex_unlock(&mutex);
}

// funcion grep;
/*1) Recibe la expresion regular.
  2) Crea, elimina y administra los hilos.
  3) cada hilo ejecuta getmessage*/
void grep(char *regular_expression, int cant_thread){
	printf(" %s %i \n", regular_expression, cant_thread);
	//creamos un arreglo de hilos
	pthread_t th[cant_thread];
	int i = 0;
	
	for( i = 0; i < cant_thread; i++){
		//creamos a los n hilos, regular_expression se lo paso asi porque como es un arreglo de chars, la variable ya es un puntero.
		if(pthread_create(&th[i], NULL, &getmessage, regular_expression) != NULL ){
			perror("failed to cretate thread \n");
		}
	}
	for( i = 0; i < cant_thread; i++){
		// le ejecutamos join sobre los n hilos
		if (pthread_join(th[i], NULL) != 0){
			perror("failed to create thread \n");
		}
	}
	
	// si aun quedan archivos por procesar vuelve a llamar a la funcion
	if( totalmessage > 0 ) grep(regular_expression, cant_thread);
	
}

int main(){
	
	//inicia el tiempo
	start = clock();
	
	//inicializacion del mutex
	pthread_mutex_init(&mutex, NULL);
	
	// insertar los archivos
    sendfilemessage("archivo_a.txt");
	sendfilemessage("file_a.txt");
	
	//llamada al grep
	grep("ipsum", 3);
	
	//finalizar el mutex
	pthread_mutex_destroy(&mutex);
	
	//caba el tiempo
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Execution time: %f\n", cpu_time_used);

    return 0;
}
