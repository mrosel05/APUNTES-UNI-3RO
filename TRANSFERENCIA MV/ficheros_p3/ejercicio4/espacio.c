#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks){
	struct stat buffer;
	if(lstat(fname, &buffer) < 0){
		fprintf(stderr, "error in lstat in get_size\n");
		return -1;
	}

	*blocks += buffer.st_blocks / 2;
	if((buffer.st_mode & S_IFMT) == S_IFDIR){
		if(get_size_dir(fname, blocks) < 0){
			fprintf(stderr, "failed to get_size_dir\n");
			return -1;
		}
	}

	return 0;
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks){
	DIR* direction;
	if((direction = opendir(dname)) == NULL){
		fprintf(stderr, "failed to opendir\n");
		return -1;
	}

	struct dirent* dir;
	while((dir = readdir(direction)) != NULL){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) continue;

		char fulldirection[1024] = "";
		snprintf(fulldirection, sizeof(fulldirection),"%s/%s", dname, dir->d_name);
		if(get_size(fulldirection, blocks) < 0){
			fprintf(stderr, "failed to get_size in get_size_dir\n");
			return -1;
		}
	}
	closedir(direction);

	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Usage %s <filename> [filename2] [...]\n", argv[0]);
		exit(1);
	}

	for(int i = 1; i < argc; i++){
		size_t size = 0;
		if(get_size(argv[i], &size) < 0){
			fprintf(stderr, "failed to calculate de size in main\n");
			exit(1);
		}
		printf("%ldK\t%s\n", size, argv[i]);
	}

	return 0;
}
