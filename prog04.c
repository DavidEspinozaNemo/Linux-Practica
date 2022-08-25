#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main () {
  int status, i, n=4;
  pid_t child_pid;
  for (i=1; i<n; ++i) if((child_pid = fork()) == -1) break;
  printf ("Process ID: %ld, Parent ID: %ld\n", (long)getpid (), (long)getppid());
  wait(&status);
  exit(0);
}
