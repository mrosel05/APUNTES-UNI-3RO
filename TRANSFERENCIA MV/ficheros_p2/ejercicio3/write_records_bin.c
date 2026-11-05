#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SimpleRecord.h"

int ids[] = {1, 0, 2};
double values[] = {3.10, 19.77, 7.42};
char* labels[] = {"Barcelona", "Madrid", "Valencia"};

int main(int argc, char* argv[]){
    SimpleRecord srv[3];
    for(int i = 0; i < 3; i++){
        srv[i].id = ids[i];
        srv[i].value = values[i];
        if(strlen(labels[i]) <= LABEL_MAX_LEN)
            strcpy(srv[i].label, labels[i]);
    }

    if(argc != 2){
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    FILE* file;
    if((file = fopen(argv[1], "w")) == NULL){
        fprintf(stderr, "Unable to open the file");
        exit(1);
    }

    for(int i = 0; i < 3; i++){
        if(fwrite(&srv[i], sizeof(SimpleRecord), 1, file) != 1){
            fprintf(stderr, "Unable to write on the file\n");
            exit(1);
        }
    }
    fclose(file);
    printf("La escritura ha finalizado correctamente\n");
    return 0;
}