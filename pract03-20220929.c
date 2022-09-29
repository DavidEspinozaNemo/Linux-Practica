#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
	
#define FOUND 0
#define NOT_FOUND 1
#define OPEN_ERROR 2
#define READ_ERROR 4
	
int main() {
    
    DIR *dirp = opendir(".");
	struct dirent* dp;
	char name[256];
	
	while (dirp) {
	    errno = 0;
	    if ((dp = readdir(dirp)) != NULL) {
			printf("filename: %s\n",dp->d_name);
	        if (strcmp(dp->d_name, name) == 0) {
	            closedir(dirp);
	            return FOUND;
	        }
	    } else {
	        if (errno == 0) {
	            closedir(dirp);
	            return NOT_FOUND;
	        }
	        closedir(dirp);
	        return READ_ERROR;
	    }
	}
	
	return OPEN_ERROR;
}