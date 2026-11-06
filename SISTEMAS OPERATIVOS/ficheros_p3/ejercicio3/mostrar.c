#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void mostrar(int fdi){
	int n;
	char buf;
	while((n = read(fdi, &buf, 1)) > 0){
		if(write(1, &buf, n) != n){
			fprintf(stderr, "An error happened while writing\n");
			exit(1);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Usage %s <filename> [-n N] [-e]", argv[0]);
		exit(1);
	}

	int offset = 0, fromBegin = 1;
	int opt;
	while((opt = getopt(argc, argv, "n:e")) > 0){
		switch (opt){
			case 'n': offset = atoi(optarg); break;
			case 'e': fromBegin = 0;
		}
	}

	if(optind >= argc){
		fprintf(stderr, "Usage %s <filename> [-n N] [-e]", argv[0]);
		exit(1);
	}

	int fdi;
	if((fdi = open(argv[optind], O_RDONLY)) < 0){
		fprintf(stderr, "Unable to read from file");
		exit(1);
	}

	int whence = fromBegin? SEEK_SET : SEEK_END;
	if(!fromBegin) offset = -offset;
	
	if(lseek(fdi, offset, whence) < 0){
    	perror("lseek");
    	exit(1);
	}

	mostrar(fdi);

	return 0;
}
