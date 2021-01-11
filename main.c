#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THREADS 4

int x = 0;

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void* A(void* arg)
{
	printf("thread A comecou\n");
	int id = *(int*)arg;
	pthread_mutex_lock(&x_mutex);
	x++;
	if(id == 0)
		printf("bom dia!\n");
	else
		printf("tudo bem?\n");
	if(x > 1)
	{
		printf("Enviando sinal de desbloqueio.\n");
		pthread_cond_broadcast(&x_cond);
	}
	pthread_mutex_unlock(&x_mutex);
	pthread_exit(NULL);
}
void* B(void* arg)
{
	printf("thread B comecou\n");
	int id = *(int*) arg;
	pthread_mutex_lock(&x_mutex);
	while( x < 2 )
	{
		printf("thread B vai se bloquear...\n");
		pthread_cond_wait(&x_cond,&x_mutex);
		printf("thread B desbloqueada.\n");
	}
	if( id == 2 )
		printf("ate mais!\n");
	else
		printf("boa tarde!\n");
	pthread_mutex_unlock(&x_mutex);
	pthread_exit(NULL);
}
int main()
{
	int ids[] = {0,1,2,3};
	pthread_t t_ids[N_THREADS];
	
	pthread_create(&t_ids[2],NULL,B,(void*) &ids[2]);
	pthread_create(&t_ids[3],NULL,B,(void*) &ids[3]);
	pthread_create(&t_ids[0],NULL,A,(void*) &ids[0]);
	pthread_create(&t_ids[1],NULL,A,(void*) &ids[1]);
	
	for(int i = 0; i < N_THREADS; i++)
		pthread_join(t_ids[i],NULL);
	printf("FIM\n");
	return 0;
}















