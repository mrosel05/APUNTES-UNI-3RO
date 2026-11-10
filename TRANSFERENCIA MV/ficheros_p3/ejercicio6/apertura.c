#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    char* fichero = NULL;
    int lectura = 0, escritura = 0;
    int opt;
    while((opt = getopt(argc, argv, "f:rw")) > 0){
        switch (opt){
            case 'f': fichero = optarg; break;
            case 'r': lectura = 1; break;
            case 'w': escritura = 1; break;
        }
    }

    if(fichero == NULL) {
        fprintf(stderr, "El fichero de entrada es obligatorio\n");
        exit(1);
    }
    if(!lectura && !escritura){
        fprintf(stderr, "Indicar lectura y/o escritura\n");
        exit(1);
    }

    int flags;
    if(lectura && escritura) flags = O_RDWR;
    else if(lectura) flags = O_RDONLY;
    else flags = O_WRONLY;

    int fd;
    if((fd = open(fichero, O_CREAT | O_TRUNC | flags, S_IRWXU)) < 0){
        fprintf(stderr, "Error al abrir el fichero\n");
        exit(1);
    }

    char buffer[]= "Prueba";
    if(write(fd, buffer, strlen(buffer)) < 0){
        fprintf(stderr, "Error al escribir\n");
    }

    char buffer2[20];
    if(read(fd, buffer2, 20) < 0) {
        fprintf(stderr, "Error al leer\n");
    }


    close(fd);
}

/*
1. Con -r no se puede escribir. Con -w no se puede leer. 
Con ambos no hay problemas. Fallan respectivamente la escritura y la lectura.

2. Al quitarle permisos, el error es al abrir. No permite abrir el 
fichero con lectura o escritura si se lo hemos quitado. 

3. Al quitarle los permisos de ejecución e intentar ejecutarlo, 
obviamente nos da error. 
*/
