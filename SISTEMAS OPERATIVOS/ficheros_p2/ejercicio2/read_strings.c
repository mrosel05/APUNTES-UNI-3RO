#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char *loadstr(FILE *file){
	int tam = 0;
	char aux;
	while(fread(&aux, 1, 1, file) == 1 && aux != '\0')
		tam++;

	if (tam == 0 && feof(file)) return NULL;
    
	if(fseek(file, -tam-1, SEEK_CUR) == -1) 
		err(2,"fseek failed");

	char* sol = malloc(sizeof(char) * (tam+1));
	if(fread(sol, 1, sizeof(char) * (tam+1), file) == 0){
		free(sol);
		err(2,"fread failed");
		exit(1);
	}
	return sol;	
}

int main(int argc, char *argv[]) {
	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	FILE* file;
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);
	
	char* str;
	while((str = loadstr(file)) != NULL){
		printf("%s\n", str);
		free(str);
	}
	fclose(file);
	return 0;
}
