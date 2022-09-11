#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  int status, i, n = 5;
  pid_t child_pid, parent_pid = getpid();
  for (i=1; i<n; ++i)
    if((child_pid = fork()) <= 0 ) break;
  for(i=1; i<n; ++i){
    if( getpid() == parent_pid){
      child_pid = wait(&status);
      printf("Child ID: %ld \n", (long) child_pid);
    }
  }
  exit(0);
}
