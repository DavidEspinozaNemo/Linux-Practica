#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main () {
  int status, i, n = 5;
  pid_t child_pid;
  for (i=1; i<n; ++i)
    if((child_pid = fork()) <= 0) break;
  printf("Process ID: %ld, Parend ID: %ld\n", (long)getpid(), (long)getppid());
  wait(&status);
  exit(0);
}
