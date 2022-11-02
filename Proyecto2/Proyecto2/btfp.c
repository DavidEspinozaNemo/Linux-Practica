// https://www.youtube.com/watch?v=zFHjKCVwS48
// https://victorroblesweb.es/2013/12/02/comandos-ftp-en-la-consola/

// https://www.binarytides.com/socket-programming-c-linux-tutorial/
// https://whatismyipaddress.com/
#include <stdio.h>
#include <stdlib.h>

void menu_connection(){
    int opcion;
    int bandera = 1;

    do{
        printf(" -*-*-*-*-*-*- Menu directory -*-*-*-*-*-*- \n\n");
        printf(" 1. close \n");
        printf(" 2. quit program \n");
        printf(" 3. cd <directory>\n");
        printf(" 4. get <archivo> \n");
        printf(" 5. lcd <directory> \n");
        printf(" 6. ls \n");
        printf(" 7. put <archivo> \n");
        printf(" 8. pwd \n");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                printf("close: cierra la conexión actual\n");
                init_menu(); //vuelve al menu init
                bandera = 0;
                break;
            case 2: 
                bandera = 0;
                printf("Finalizando programa . . . \n");
                break;
            case 3: 
                printf("cd <directorio>: cambia de directorio remoto\n");
                break;
            case 4: 
                printf("get <archivo>: recupera un archivo remoto\n");
                break;
            case 5: 
                printf("lcd <directorio>: cambia de directorio local\n");
                break;
            case 6: 
                printf("ls: lista los archivos del directorio remoto\n");
                break;
            case 7: 
                printf("put <archivo>: envía un archivo a la máquina remota\n");
                break;
            case 8: 
                printf("pwd: muestra el directorio activo remoto\n");
                break;
            default:
                printf("Opcion no valida, vuelva a intentar.\n");
                break;
        }
    } while(bandera);
}

void init_menu(){
    int opcion;
    int bandera = 1;

    do{
        printf(" -*-*-*-*-*-*- Menu -*-*-*-*-*-*- \n\n");
        printf(" 1. Open directory-ip \n");
        printf(" 2. quit program      \n");
        /* aqui muestro la informacion de los otros sockets, si
           estan atendiendo o en espera y cuantos ahi. */
        scanf("%d", &opcion);

        switch(opcion){
            case 1:    
                menu_connection();
                printf("open <dirección-ip>: establece una conexión remota\n");
                bandera = 0;
                break;
            case 2: 
                bandera = 0;
                printf("Finalizando programa . . . \n");
                break;
            default:
                printf("Opcion no valida, vuelva a intentar.\n");
                break;
        }
    } while(bandera);
}

int main() {
    init_menu();
    return 0;
}
