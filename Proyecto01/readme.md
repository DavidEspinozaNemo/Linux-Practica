# Proyecto 01 - Principio de Sistemas Operativos IC6600

## Integrantes
```
  David Jose Espinoza Soto - 2016012024
  Junior Eduardo Segura Ballestero - 2019390109
```

## Introducción

El objetivo de este proyecto es comparar el rendimiento de realizar una tarea colaborativa utilizando múltiples hilos. Para ello se deberá desarrollar una versión multihilos del programa llamado grep que permite buscar palabras sobre varios archivos e imprimir el archivo en donde aparece.

En este proyecto se deben realizar múltiples pruebas para determinar la cantidad óptima de hilos para ejecutar este tipo de tarea sobre gran cantidad de archivos.

### Uso del programa


El programa grep recibe una expresión regular y una lista de archivos:

```
    grep [expresión regular] archivo1 archivo2 archivo3 ...
```

Por omisión el programa muestra la línea del archivo en donde se encuentra la expresión regular.

### Manejo de expresiones regulares

El lenguaje C por sí mismo no maneja expresiones regulares, sin embargo se puede usar la librería "regex.h" que cuenta con las funciones regcomp y regexec que permiten realizar esta tarea. La función regcomp se debe ejecutar inicialmente y recibe la expresión regular ingresada por comando, luego la función regexec se debe ejecutar sobre cada una de las líneas del archivo.

### Manejo de múltiples hilos

El programa debe contar con múltiples hilos que se encarguen de inspeccionar los diferentes archivos. Debido a que la búsqueda puede abarcar una gran cantidad de archivos no sería eficiente crear un hilo por cada archivo. Es por ello que es necesario crear un conjunto compartido de hilos (pool de hilos) que incluya solo una cantidad limitada de hilos. Como parte del proyecto se deben realizar diferentes pruebas (con muchos archivos) para determinar un tamaño "óptimo" del pool de hilos. Se utilizará el tiempo de ejecución para determinar dicho tamaño óptimo

En este caso se utilizará un pool de hilos estático, es decir, el programa creará la totalidad de los hilos al inicio del programa y únicamente utilizará estos para realizar todo el trabajo. Tal como usted debe saber, a los hilos que terminen de buscar en un archivo se les deberá asignar otro archivo para realizar la búsqueda hasta terminar con todos los archivos.

## Definición de estructuras de datos

El proyecto consta de funciones, la primera es el main donde se insertan los nombre de los archivos que se van a usar, luego se llama a la función grep con la expresión regular y la cantidad de hilos a usar, la funcón sendfilemessage envia a la cola el nombre de los archivos que vamos a utilizar, la función grep es quien crea y administra los hilos, y cada uno de estos hilos ejecuta la función search_regular_expression.

```c
struct filemessage {
    long type;
    char namefile[MSGSZ];
}msg;
```

Esta es la estructura donde se almacenan los nombres de los archivos donde se van a buscar las expreciones regulares.

## Descripción detallada y explicación de los componentes principales del programa

La función sendfilemessage resive una cadena de caracteres ( la expreción regular ), y la envia a una cola de mensages con la instrucción msgsnd, internamente maneja el total de mensajes enviados con la variable totalmessage.

```c
void sendfilemessage(char *mensagetxt){
    key_t msqkey = 999;
    int msqid = msgget(msqkey, IPC_CREAT | S_IRUSR | S_IWUSR);
    msg.type = 1;
    strcpy(msg.namefile, mensagetxt);
    msgsnd(msqid,(void *)&msg, sizeof(msg.namefile) ,IPC_NOWAIT);
    printf("Message sent\n");
    totalmessage++;
}
```
La función search_regular_expression resive dos cadenas de caracteres el nombre del archivo de texto, y la expresión regular. Primero crea las variableas que va a utilizar, intenta abrir el archivo en modo lectura, intenta crear el regex, y al final leer el archivo linea x linea hasta llegar al final, y utilizando el regex va buscando la expresion regular.

```c
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
```
La funcion getmessage resive la expresión regular, y es la función que va a ser llamada por cada hilo. Mientras la cantidad de mensages en la cola sea mayor que cero, aplicara la función search_regular_expression.

```c
void getmessage(char *regular_expression){
    key_t msqkey = 999;
    int msqid = msgget(msqkey, IPC_CREAT | S_IRUSR | S_IWUSR);
    msg.type = 1;

    if( totalmessage > 0 ){
        msgrcv(msqid, &msg, MSGSZ, 1, 0);
        if ( msgrcv == NULL ) exit(0); //si o recibe el mensaje se va.
        printf("Message received, file %s in process\n",msg.namefile);

	totalmessage--;
	search_regular_expression( msg.namefile, regular_expression);	
    }
    else{
    	printf("No message \n");
    }
}
```

La función grep es quien administra los hilos, crea un conjunto de n hilos y a cada uno le pasa la expresión regular que deben buscar en los archivos, utiliza un mutex para bloquear los recursos, y la funcion getmessage al intentar obtener más archivos de los que existen simplemente se bloquea.

```c
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
```

## Pruebas de rendimiento

Nuestro mecanismo para medir el tiempo fue con la libreria time, utilizamos 6 archivos con un texto obtenido del libro Don Quijote de la Mancha, y buscamos la misma expresión regular pero alterando el numero de hilo, la siguiente tabla muestra el promedio del tiempo de ejecución por cada 10 pruebas contraladas.

|  Cantidad de hilos          | Promedio de tiempo ejecución |
|-----------------------------|:----------------------------:|
|12 hilos                     |     0,0013386                |
| 6 hilos                     |     0,0006174                |
| 5 hilos                     |     0,0008504                |
| 4 hilos                     |     0,0006927                |
| 3 hilos                     |     0,0005530                |
| 2 hilos                     |     0,0005029                |

## Conclusiones

Programar en Linux es toda una experiencia, tubimos problemas desde el comando **chmod +x file.txt** para darle los permisos de lectura a los archivos de texto utilizados, hasta la forma de compilar un programa en c que utiliza hilos, yo use el comando **gcc -o ejecutable programa.c -lpthread** para mis pruebas.

podemos concluir que el numero opimo, para este algoritmo, son 2 hilos.

