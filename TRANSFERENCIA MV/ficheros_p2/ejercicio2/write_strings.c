#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[])  {
	if (argc < 3) {
    fprintf(stderr, "Usage: %s <file_name> <string1> [string2 ...]\n", argv[0]);
    exit(1);
	}

	FILE* file;
	if((file = fopen(argv[1], "w")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	int i = 2;
	while(i < argc){
		int ret = fwrite(argv[i], 1, strlen(argv[i])+1, file);
		if (ret < strlen(argv[i])+1){fclose(file);err(3,"fwrite() failed!!");}
		i++;
	}

	fclose(file);
	return 0;
}
