#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
	
int main (int argc, char* argv[]) {
    char* file = argv[1];
	int fd;
	struct flock lock;
    int i = 2; //programa par
    char n[100];
    int max_len = sizeof n;

    FILE *f;
	
    printf ("opening %s\n", file);
	
	fd = open (file, O_WRONLY);
	printf ("locking\n");
	
	memset (&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK;
	/* Place a write lock on the file. */
    f = fopen(file, "a");
    while(i < 1000){
        snprintf(n, max_len, "%d %d %d %d %d %d %d %d %d %d\n", i+2,i+4,i+6,i+8,i+10,i+12,i+14,i+16,i+18,i+20);
        fputs(n, f);
        i = i + 20;
        sleep(3);
    }
    fclose(f);
    /* ** ** */
	fcntl (fd, F_SETLKW, &lock);
    
	printf ("unlocking\n");
	
	lock.l_type = F_UNLCK;
	fcntl (fd, F_SETLKW, &lock);
    
	close (fd);
	return 0;
}