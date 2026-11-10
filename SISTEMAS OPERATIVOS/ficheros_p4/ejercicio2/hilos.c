#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct arg{
	int numHilo;
	char prioridad;
};

void *thread_usuario(void *arg){
	struct arg* arg1 = (struct arg*) arg;

	int nh = arg1->numHilo;
	int prio = arg1->prioridad;
	pthread_t self = pthread_self();
	free(arg1);

	printf("%u. ID=%ld\tprioridad=%c\n", nh, (unsigned long)self, prio);
}

int main(int argc, char* argv[]){
	int n = 10;
	pthread_t hilos[n];
	for(int i = 0; i < n; i++){
		struct arg* arg1 = (struct arg*) malloc(sizeof(struct arg));
		arg1->numHilo = i;
		arg1->prioridad = (i % 2 == 0)? 'P':'N';

		pthread_create(&hilos[i], NULL, thread_usuario, arg1);
	}

	for(int i = 0; i < n; i++){
		pthread_join(hilos[i], NULL);
	}

	return 0;
}
