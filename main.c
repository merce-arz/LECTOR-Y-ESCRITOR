#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int numreader = 0;

void *escritor(void *wno)
{
    sem_wait(&wrt);
    cnt = cnt*2;
    printf("Escritor %d esta escribiendo \n",(*((int *)wno)),cnt);
    sem_post(&wrt);

}
void *lector(void *rno)
{
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt);
    }
    pthread_mutex_unlock(&mutex);
    printf("Lector %d: esta leyendo \n",*((int *)rno),cnt);
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_t lee[8],escribe[6];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[10] = {1,2,3,4,5,6,7,8,9,10};

    for(int i = 0; i < 8; i++) {
        pthread_create(&lee[i], NULL, (void *)lector, (void *)&a[i]);
    }
    for(int i = 0; i < 6; i++) {
        pthread_create(&escribe[i], NULL, (void *)escritor, (void *)&a[i]);
    }

    for(int i = 0; i < 6; i++) {
        pthread_join(lee[i], NULL);
    }
    for(int i = 0; i < 6; i++) {
        pthread_join(escribe[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;

}
