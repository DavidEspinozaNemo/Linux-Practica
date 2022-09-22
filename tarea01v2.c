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

//David Jose Espinoza Soto -  2016012024
/* Tarea 01 - Esta tarea consiste en desarrollar un programa que cree archivos
de backup (.bak) de una lista de archivos que se pasan como parámetro.
Para ello deberán crear procesos hijos que ejecuten el proceso de copiado.
*/
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

//funcion para recibir un mensage, y de paso hacer el .bak
void receivemessage(){
  key_t msqkey = 999;
  int msqid = msgget(msqkey, IPC_CREAT | S_IRUSR | S_IWUSR);
  msg.type = 1;
  msgrcv(msqid, &msg, MSGSZ, 1, 0);
  if ( msgrcv == NULL ) exit(-1); //si o recibe el mensaje se va.
  printf("Message received, file  %s in process\n",msg.namefile);
  printf("Process ID: %ld, Document'name to process: %s \n", (long)getpid(), msg.namefile);

  FILE *fptr1, *fptr2;
  char *namereturn, c;
  int i = 1;


  namereturn = msg.namefile;

  while( namereturn[i] != '\0'){
      if( namereturn[i] == '.') {
      namereturn[i+1] = 'b';
      namereturn[i+2] = 'a';
      namereturn[i+3] = 'k';
      }
      i++;
  }

  fptr1 = fopen(msg.namefile, "r");
  if (fptr1 == NULL) {
    printf("Process ID: %ld, Document %s in mode R cannot open. \n", (long)getpid(), msg.namefile);
    exit(-1);
  }
  fptr2 = fopen(namereturn, "w");
  if (fptr2 == NULL) {
      printf("Process ID: %ld, Document %s in mode R W cannot open. \n", (long)getpid(), namereturn );
      exit(-1);
  }

  //copiado del archivo
  c = fgetc( fptr1 );
  while( c != EOF ){
      fputc(c, fptr2);
      c = fgetc(fptr1);
  }
  //if( (fptr1 != NULL)&&(fptr2 != NULL) ) totalmessage--;
  totalmessage--;
  fclose(fptr1);
  fclose(fptr2);
}

// esta funcion toma todos los mensages en la cola, y les aplica el receivemessage
void backend(){
    int status,i,n;
    pid_t child_pid, parent_pid=getpid();
    n = totalmessage;
    for (i=0; i<n; ++i){
        if( totalmessage > 0 ){
            fork();
            receivemessage();
        }
        else break;

        child_pid = getpid();
        if( child_pid <=0 ) break;
    }
    for (i=1; i<n; ++i)
        if (getpid()==parent_pid) {
            child_pid = wait(&status);
            printf ("Child ID: %ld\n", (long)child_pid);
        }
}

int main(){

    sendfilemessage("filea.txt");
    sendfilemessage("fileb.txt");
    sendfilemessage("filec.txt");
    backend();

    return 0;
}
