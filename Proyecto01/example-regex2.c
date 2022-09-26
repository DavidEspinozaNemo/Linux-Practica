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
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

void search_regular_expression (char *regular_expression){
	
	printf("%s \n", regular_expression);
	FILE *fptr;
	char *sentence = NULL;
	char *s = regular_expression;
    size_t len = 0;
    ssize_t read;
	
	regex_t     regex;
    regmatch_t  pmatch[1];
    regoff_t    off, lenn;
		
	//Abre el archivo llamado msg.namefile
	fptr = popen("./archivo_a.txt", "r");
		
	if( fptr != NULL ){ //si pudo abrir el archivo
		
		//lectura linea x linea del archivo	
			
		while ((read = getline(&sentence, &len, fptr)) != -1) {
			printf("%s", sentence);
			
			if (regcomp(&regex, regular_expression, REG_NEWLINE)) exit(EXIT_FAILURE);
			
			for (int i = 0; ; i++) {
				if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))
					break;

				off = pmatch[0].rm_so + (s - regular_expression);
				lenn = pmatch[0].rm_eo - pmatch[0].rm_so;

				printf("offset = %jd;\n", (intmax_t) off);

				s += pmatch[0].rm_eo;
			}
		}
		
		fclose(fptr); //cerrando el archivo
	} //si no pudo abrir el archivo
	else{
		printf("file archivo_a.txt can't be opened \n");
	}
}

int main(){
	char *expression = "hola mundo";
	search_regular_expression(expression);
}