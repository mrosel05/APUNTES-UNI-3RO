#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char **parse_command(const char *cmd, int* argc);

pid_t launch_command(char** argv){
    pid_t id = fork();

    if(id < 0){
        perror("fork");
        return -1;
    }
    if(id == 0){
        execvp(argv[0], argv);
        exit(0);
    }

    return id;
}

void free_argv(char** argv){
    for (int i = 0; argv[i] != NULL; i++) free(argv[i]);  
    free(argv);
}

void process_command(char* command, int allAtOnce, pid_t pids[], int *num_commands){
    char **cmd_argv;
    int cmd_argc;
    cmd_argv=parse_command(command,&cmd_argc);
    pid_t id = launch_command(cmd_argv);
    free_argv(cmd_argv);
    if(allAtOnce){
        pids[*num_commands] = id;
        (*num_commands)++;
    }else{
        int status;
        waitpid(id, &status, 0);
        printf("@@ Command #%d terminated (pid: %d, status: %d)\n",*num_commands,id,WEXITSTATUS(status));
        (*num_commands)++;
    } 
}

void process_file(char* filename, int allAtOnce, pid_t pids[], int *num_commands){
    FILE* file;
    if((file = fopen(filename, "r")) == NULL){
        perror("fopen");
        exit(1);
    }

    char linea[1024];

    while (fgets(linea, sizeof(linea), file) != NULL) {
        printf("@@ Running command #%d: %s", *num_commands, linea);
        process_command(linea, allAtOnce, pids, num_commands);
    }

    fclose(file);
}




char **parse_command(const char *cmd, int* argc) {
    // Allocate space for the argv array (initially with space for 10 args)
    size_t argv_size = 10;
    const char *end;
    size_t arg_len; 
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));

    if (argv == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start)) start++; // Skip leading spaces

    while (*start) {
        // Reallocate more space if needed
        if (arg_count >= argv_size - 1) {  // Reserve space for the NULL at the end
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        // Find the start of the next argument
        end = start;
        while (*end && !isspace(*end)) end++;

        // Allocate space and copy the argument
        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);

        if (argv[arg_count] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0';  // Null-terminate the argument
        arg_count++;

        // Move to the next argument, skipping spaces
        start = end;
        while (*start && isspace(*start)) start++;
    }

    argv[arg_count] = NULL; // Null-terminate the array

    (*argc)=arg_count; // Return argc

    return argv;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <\"command\" | filename> [-b]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* command = NULL;
    char* filename = NULL;
    int allAtOnce = 0;
    int opt;
    while((opt = getopt(argc, argv, "x:s:b")) > 0){
        switch (opt){
        case 'x': command = optarg; break;
        case 's': filename = optarg; break;
        case 'b': allAtOnce = 1; break;
        }
    }

    if(command == NULL && filename == NULL){
        perror("Give a command or a filename");
        exit(1);
    }

    pid_t pids[100];
    int num_commands = 0;

    if(command != NULL){
        process_command(command, 0, pids, &num_commands);
    }

    if(filename != NULL){
        process_file(filename, allAtOnce, pids, &num_commands);
        if(allAtOnce){
            int status;
            pid_t finished;

            while ((finished = wait(&status)) > 0) {
                int command_number = -1;
                for (int i = 0; i < num_commands; i++) {
                    if (pids[i] == finished) {
                        command_number = i;
                        break;
                    }
                
                    printf("@@ Command #%d terminated (pid: %d, status: %d)\n",
                        command_number,finished,WEXITSTATUS(status));
                }
            } 
        }
    }
    return EXIT_SUCCESS;    
}