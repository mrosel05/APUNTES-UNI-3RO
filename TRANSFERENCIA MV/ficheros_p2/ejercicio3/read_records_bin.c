#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SimpleRecord.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    FILE* file;
    if((file = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "Unable to open the file");
        exit(1);
    }

    SimpleRecord srv[3];
    int cont = 0;
    while(cont < 3 && fread(&srv[cont], sizeof(SimpleRecord), 1, file) == 1){
        cont++;
    }
        
    for(int i = 0; i < 3; i++){
        printf("ID:%d, Valor:%'.2f, Etiqueta:%s\n", srv[i].id, srv[i].value, srv[i].label);
    }
    fclose(file);
    return 0;
}