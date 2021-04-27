#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 5

pthread_mutex_t mutex;
unsigned available_resources;

/*Las siguientes infunciones fueron brindadas por el documento del laboratorio*/
/*Fueron modificados con el fin de mostrar de mejor manera los pasos de la ejecucion*/

/* decrease available resources by count resources */
/* return 0 if sufficient resources available, */
/* otherwise return -1 */
int decrease_count(int count){
	if (available_resources < count) {
		return -1;
	}
	else{
		printf("\n");
		pthread_mutex_lock(&mutex);
		printf("Se bloquea Mutex \n");
		printf("Si hay recursos disponibles, consumiendo %d \n", count);
		available_resources = available_resources - count;

		pthread_mutex_unlock(&mutex);
		printf("Desbloqueando Mutex \n");
		return 0;
	}
}

/* increase available resources by count */
int increase_count(int count){
	pthread_mutex_lock(&mutex);
	printf("Se bloquea Mutex \n");

	printf("Regresando la cantidad de %d recursos \n", count);
	available_resources = available_resources + count;

	pthread_mutex_unlock(&mutex);
	printf("Desbloqueando Mutex \n");
	return 0;
}

void* threads_to_do(void) {
	int i;

	pid_t id;
	id = syscall(SYS_gettid);

	printf("\n");
	printf("Thread proces id %d \n", id);

	int res_consume = (rand() % 11) + 5;

	printf("Id actual: %d - Tomando %d recursos \n", id, res_consume);

	while (decrease_count(res_consume) != 0) {
		NULL;
	}
	decrease_count(res_consume);
	int time = (rand()%2)+1;
	sleep(time);

	printf("\n");
	printf("Id actual: %d - Terminando de usar %d recursos \n", id, res_consume);

	increase_count(res_consume);

	printf("Id actual: %d - Se devolvieron %d recursos \n", id, res_consume);

}

int main() {
	FILE *fptr;
	fptr = fopen("Log2.txt", "w");

	if (fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}

	fprintf(fptr, "Iniciando programa\n");
	int j;
	pthread_t *thread;

	available_resources = 20;

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
