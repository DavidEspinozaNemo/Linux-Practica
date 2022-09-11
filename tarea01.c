#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct book{
  char *name;
  struct book *next;
};

void print_books(struct book *head);
void push(struct book *head, char *name_book);
char* pop(struct book **head);
struct book* create_book(char *name_book);

void backup(struct book *list_file);
void backup_aux(char *name_book);

int main(){
  // creo la lista de archivos
  struct book *list_file = create_book("z.txt");
  // push de los archivos existentes
  push(list_file, "a.txt");
  push(list_file, "b.txt");
  push(list_file, "c.txt");
  print_books(list_file);
  // llama al backup
  backup(list_file);

  return 0;
}

void print_books(struct book *head){
  struct book *current = head;

  while(current != NULL){
    printf("book name: %s \n", current->name);
    current = current->next;
  }
}

struct book* create_book(char *name_book){
  struct book *head = (struct book *) malloc(sizeof(struct book));
  head->name = name_book;
  head->next = NULL;

  return head;
}

//solo funciona para listas ya existentes
void push(struct book *head, char *name_book){
  struct book *current = head;

  while ( current->next != NULL ) current = current->next;

  current->next = (struct book *) malloc(sizeof(struct book));
  current->next->name = name_book;
  current->next->next = NULL;
}

char* pop(struct book **head){
  char *name = NULL;
  struct book *current = *head;

  if(*head == NULL) return NULL;

  name = (*head)->name;
  *head = current->next;

  free(current);

  return name;
}

void backup(struct book *list_file){
  int status, count = 1;
  char *archivos = NULL;
  struct book *current = list_file;
  pid_t child_pid, parent_pid = getpid();

  while( current->next == NULL ){
    archivos = pop(&current);
	
	child_pid = fork();
	backup_aux(archivos);

    current = current->next;
	count++;
	
	if( getpid() <= 0 ) break;
  }
  // esperar a que todos lo hijos terminen
  while( count-- <= 0){
    if( getpid() == parent_pid){
	  child_pid = wait(&status);
	}
  }
  free(current);
}

void backup_aux(char *name){
  printf("Process ID: %ld, Document name: %s \n", (long)getpid(), name);
  
  FILE *fptr1, *fptr2;
  char *namereturn, c;
  int i = 1;
  
  namereturn = name[0];
  while( name[i] != '\0'){
	  if( name[i] == '.') namereturn += '.bak';
	  else namereturn += name[i];
	  i++;
  }
  
  fptr1 = fopen(name, "r");
  if (fptr1 == NULL) printf("Process ID: %ld, Document R cannot open. \n", (long)getpid() );
  
  fptr2 = fopen(namereturn, "w");
  if (fptr1 == NULL) printf("Process ID: %ld, Document W cannot open. \n", (long)getpid() );
  
  //copiado del archivo
  c = fgetc( fptr1 );
  while( c != EOF ){
	  fputc(c, fptr2);
	  c = fgetc(fptr1);
  }
}

//notas: aun no me funciona que los hijos puedan realizar el trabajo de copiar el archivo.
