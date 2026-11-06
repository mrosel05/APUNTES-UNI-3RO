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

void copy_regular(char *orig, char *dest){
	int fdi, fdo;
	if((fdi = open(orig, O_RDONLY)) < 0 || (fdo = open(dest, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU)) < 0){
		fprintf(stderr, "Unable to open File\n");
		exit(1);
	}

	copy(fdi, fdo);

	close(fdi);
	close(fdo);
}

void copy_link(char *orig, char *dest){
	struct stat buffer;
	if(lstat(orig, &buffer) < 0){
		fprintf(stderr, "Unable to do lstat");
		exit(1);
	}

	int size = buffer.st_size + 1;
	char* route = (char*) malloc(size);
	readlink(orig, route, size);
	route[size - 1] = '\0';

	symlink(route, dest);
}

int main(int argc, char *argv[]){
	if(argc < 3){
		fprintf(stderr, "Usage %s <infile> <outfile>\n", argv[0]);
		exit(1);
	}

	struct stat buffer;
	if(lstat(argv[1], &buffer) < 0){
		fprintf(stderr, "Unable to do lstat");
		exit(1);
	}

	if((buffer.st_mode & S_IFMT) == S_IFLNK)
		copy_link(argv[1], argv[2]);
	else if((buffer.st_mode & S_IFMT) == S_IFREG) 
		copy_regular(argv[1], argv[2]);
	
	return 0;
}

