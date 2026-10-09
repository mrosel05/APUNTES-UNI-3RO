#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "SimpleRecord.h"

int readbin(FILE* file, SimpleRecord srv[]){
    int cont = 0;
    while(cont < 3 && fread(&srv[cont], sizeof(SimpleRecord), 1, file) == 1)
        cont++;
}

int readtext(FILE* file, SimpleRecord srv[]){
    char c[100];
    int aux = 3, cont = 0;
    while(cont < 3 && (aux = fscanf(file, "%d %lf %s", &srv[cont].id, &srv[cont].value, c)) == 3){
        if(strlen(c) <= LABEL_MAX_LEN) strcpy(srv[cont].label, c);
        cont++;
    }
    return 0;
}

int writebin(FILE* file, SimpleRecord srv[]){
    for(int i = 0; i < 3; i++){
        if(fwrite(&srv[i], sizeof(SimpleRecord), 1, file) != 1){
            fprintf(stderr, "Unable to write on the file\n");
            exit(1);
        }
    }
    return 0;
}

int writetext(FILE* file, SimpleRecord srv[]){
    for(int i = 0; i < 3; i++){
        fprintf(file, "%d %'.2f %s\n", srv[i].id, srv[i].value, srv[i].label);
    }
    return 0;
}

int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(stderr, "Usage: %s [-i t|b] [-o t|b] <file_name> <file_name> \n", argv[0]);
        exit(1);
    }
    
    int opt;
    char infile = 't', outfile = 't';
    while((opt = getopt(argc, argv, "i:o:")) != -1){
        if(optarg[0] != 'b' && optarg[0] != 't') {
            fprintf(stderr, "Non parseable element\n");
            exit(1);
        }
        switch (opt){
        case 'i': infile = optarg[0];
            break;
        case 'o': outfile = optarg[0];
            break;
        }
    }

    FILE* in;
    if(strcmp(argv[optind], "-") == 0) {
        in = stdin;
    }
    else {
        if((in = fopen(argv[optind], "r")) == NULL){
            fprintf(stderr, "Unable to open the infile");
            exit(1);
        }
    }
    
    SimpleRecord srv[3];
    if(infile == 't') readtext(in, srv);
    else readbin(in, srv);
    
    if(strcmp(argv[optind], "-") != 0) {
        fclose(in);
    }
    


    FILE* out;
    if(strcmp(argv[optind + 1], "-") == 0){
        out = stdout;
    }
    else{
        if((out = fopen(argv[optind + 1], "w")) == NULL){
            fprintf(stderr, "Unable to open the outfile");
            exit(1);
        }
    }
    if(outfile == 't') writetext(out, srv);
    else writebin(out, srv);

    if(strcmp(argv[optind + 1], "-") != 0){
        fclose(out);
    }
    

    return 0;
}