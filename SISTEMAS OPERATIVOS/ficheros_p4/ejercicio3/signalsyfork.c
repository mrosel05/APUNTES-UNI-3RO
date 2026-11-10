#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int pid_hijo;

/*Aprograma que temporiza la ejecución de un proceso hijo */
void manejador(int sig){
	kill(pid_hijo, SIGKILL);
}

int main(int argc, char **argv){
	if(argc < 2){
		fprintf(stderr, "Usage %s command", argv[0]);
		exit(1);
	}

	pid_hijo = fork();


	if(pid_hijo == 0){ //hijo
		execvp(argv[1], &argv[1]);

		perror("execvp");
		exit(1);
	}

	signal(SIGINT, SIG_IGN);

	struct sigaction sa;
	sa.sa_handler = manejador;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &sa, NULL);

	alarm(5);

	int status;
	int ret = waitpid(pid_hijo, &status, 0);

	if (WIFEXITED(status))
    	printf("El hijo terminó normalmente con código %d\n",WEXITSTATUS(status));
	if (WIFSIGNALED(status)) 
		printf("El hijo terminó por la señal %d\n",WTERMSIG(status));
	return 0;
}
