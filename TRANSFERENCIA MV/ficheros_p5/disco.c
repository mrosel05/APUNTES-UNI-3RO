#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 2
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

int turno = 0, turno_vip = 0;
int ticket = 0, ticket_vip = 0;
int vip_esperando = 0, clientes_dentro = 0;

pthread_mutex_t m;
pthread_cond_t normal, vip;

struct client_args{
	int id, isvip;
};

void enter_normal_client(int id){
	pthread_mutex_lock(&m);

	int miTicket = ticket++;
	printf("Client %2d (%s) waiting\n", id, VIPSTR(0));

	while(vip_esperando || clientes_dentro >= CAPACITY || miTicket != turno)
		pthread_cond_wait(&normal, &m);

	clientes_dentro++;
	turno++;
	pthread_cond_broadcast(&normal);
	pthread_mutex_unlock(&m);
	printf("Client %2d (%s) entering\n", id, VIPSTR(0));
}

void enter_vip_client(int id){
	pthread_mutex_lock(&m);

	vip_esperando++;
	int miTicket = ticket_vip++;
	printf("Client %2d (%s) waiting\n", id, VIPSTR(1));

	while(clientes_dentro >= CAPACITY || miTicket != turno_vip)
		pthread_cond_wait(&vip, &m);

	vip_esperando--;
	clientes_dentro++;
	turno_vip++;
	
	if(vip_esperando) pthread_cond_broadcast(&vip);
	else pthread_cond_broadcast(&normal);
	pthread_mutex_unlock(&m);
	printf("Client %2d (%s) entering\n", id, VIPSTR(1));
}

void dance(int id, int isvip){
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip){
	pthread_mutex_lock(&m);

	printf("Client %2d (%s) exiting\n", id, VIPSTR(isvip));
	clientes_dentro--;

	if(vip_esperando) pthread_cond_broadcast(&vip);
	else pthread_cond_broadcast(&normal);
	pthread_mutex_unlock(&m);
}

void *client(void *arg){
	struct client_args* args = (struct client_args*) arg;
	int id = args->id, isvip = args->isvip;
	free(args);
	if(isvip) enter_vip_client(id);
	else enter_normal_client(id);

	dance(id, isvip);
	disco_exit(id, isvip);
}

int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Usage %s <filename>", argv[0]);
		exit(1);
	}

	FILE* file;
	if((file = fopen(argv[1], "r")) == NULL){
		perror("Unable to open file");
		exit(1);
	}

	int M;
	fscanf(file, "%d", &M);
	int clientes[M];
	for(int i = 0; i < M; i++){
		fscanf(file, "%d", &clientes[i]);
	}

	fclose(file);

	pthread_t threads[M];
	for(int i = 0; i < M; i++){
		struct client_args *arg = malloc(sizeof(struct client_args));
		arg->id  = i; arg->isvip = clientes[i];
		if(pthread_create(&threads[i], NULL, client, arg) != 0){
			perror("Unable to create thread");
			exit(1);
		}
	}

	for(int i = 0; i < M; i++){
        pthread_join(threads[i], NULL);
    }

	pthread_mutex_destroy(&m);
    pthread_cond_destroy(&normal);
    pthread_cond_destroy(&vip);
	return 0;
}
