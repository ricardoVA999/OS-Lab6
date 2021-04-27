#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 5
#define REP 4

int counter = 5000;
sem_t mut;

void* threads_to_do(void) {
	int i;

	pid_t id;
	id = syscall(SYS_gettid);

	printf("Id actual: %d - Semaforo creado \n", id);

	for (i = 0; i < REP; i++) {
		sem_wait(&mut);

		printf("Repeticion numero %d \n", i + 1);

		counter = counter - 1;

		printf("Id actual: %d - Tomando un recurso \n", id);

		int time = (rand() % 2) + 1;
		sleep(time);

		printf("Id actual: %d - Terminando de usar el recurso \n", id);

		counter = counter + 1;

		printf("Id actual: %d - Se devolvio el recurso \n", id);

		sem_post(&mut);
	}
}

int main() {
	FILE *fptr;
	fptr = fopen("Log1.txt", "w");

	if (fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}

	fprintf(fptr, "Iniciando programa\n");
	int j;
	pthread_t *thread;

	sem_init(&mut, 0, 1);

	fprintf(fptr, "Creando %d threads\n", THREADS);
	for (j = 0; j < THREADS; j++) {
		pthread_create(&thread[j], NULL, threads_to_do, NULL);
	}

	fprintf(fptr, "Esperando threads \n");
	for (j = 0; j < THREADS; j++) {
		pthread_join(thread[j], NULL);
	}

	fprintf(fptr, "Fin del programa \n");
	fclose(fptr);
	return 0;
}
