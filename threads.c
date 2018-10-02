#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void handshake(void) 
{
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void* even_thread(void* data)
{
    int limit = *(int*)data;

    /* wait for odd thread to start */
    pthread_cond_wait(&cond, &mutex);

    for (int i = 0; i < limit; i=i+2) {
        pthread_mutex_lock(&mutex);
        printf("%d ", i);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        pthread_cond_wait(&cond, &mutex);
    }

    /* signal odd thread to complete */
    handshake();
}

void* odd_thread(void* data)
{
    int limit = *(int*)data;

    /* signal even thread */
    handshake();

    /* odd thread waits for even to start printing */
    pthread_cond_wait(&cond, &mutex);

    for (int i = 1; i < limit; i=i+2) {
        pthread_mutex_lock(&mutex);
        printf("%d ", i);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        pthread_cond_wait(&cond, &mutex);
    }

}

int main()
{
    pthread_t t1, t2;
    int limit = 20;
   
    pthread_create(&t1, NULL, even_thread, (void*) &limit);
    pthread_create(&t2, NULL, odd_thread, (void*) &limit);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
