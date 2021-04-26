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
	int tmp;

	pid_t id;
	id = syscall(SYS_gettid);

	printf("%d - Semaforo abierto con exito. \n", id);

	for (i = 0; i < REP; i++) {
		sem_wait(&mut);

		printf("Iniciando iteracion %d \n", i + 1);

		counter = counter - 1;

		printf("%d - (!) Recurso tomado. \n", id);

		int time = (rand() % 2) + 1;
		sleep(time);

		printf("%d - Termine de utilizar el rescurso. \n", id);

		counter = counter + 1;

		printf("%d - Recurso devuelto! :D \n", id);

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

	fprintf(fptr, "Iniciando programa \n");
	int j;
	pthread_t *thread;

	sem_init(&mut, 0, 1);

	fprintf(fptr, "Creando threads \n");
	for (j = 0; j < THREADS; j++) {
		pthread_create(&thread[j], NULL, threads_to_do, NULL);
	}

	fprintf(fptr, "Esperando threads \n");
	for (j = 0; j < THREADS; j++) {
		pthread_join(thread[j], NULL);
	}

	fprintf(fptr, "Fin de ejecucion \n");
	fclose(fptr);
	return 0;
}