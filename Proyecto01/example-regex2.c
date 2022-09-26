#include <stdint.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

void search_regular_expression (char *file_name, char *regular_expression){
	
	printf("%s \n", regular_expression);
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
    exit(EXIT_SUCCESS);
}

int main(){
	char *expression = "ipsum";
    char *file_name = "file-a.txt";
	search_regular_expression(file_name, expression);
}
