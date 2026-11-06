#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void copy(int fdi, int fdo){
	int n;
	char buf[512];
	while((n = read(fdi, &buf, 512)) > 0){
		if(write(fdo, &buf, n) != n){
			fprintf(stderr, "An error happened while writing\n");
			exit(1);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc < 3){
		fprintf(stderr, "Usage %s <infile> <outfile>\n", argv[0]);
		exit(1);
	}

	int fdi, fdo;
	if((fdi = open(argv[1], O_RDONLY)) < 0 || (fdo = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU)) < 0){
		fprintf(stderr, "Unable to open File\n");
		exit(1);
	}

	copy(fdi, fdo);

	close(fdi);
	close(fdo);

	return 0;
}
