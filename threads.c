#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

/* pthread_cond_wait() unlocks the mutex before it sleeps.
   When it is woken up (due to signal), it reacquires the mutex - which may require waiting again.
   So, the thread would continue only after a) signaling and b) mutex unlocked by signaling thread.

   Because of the above behavior, its recommended to:
   a) signal and wait within mutex lock and unlock.
   b) wait within if condition, preferably a while loop on a condition variable.
	This condition variable ensures waiting only when needed.
 */

pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool even = true;

void* even_thread(void* data)
{
    int limit = *(int*)data;

    for (int i = 0; i < limit; i=i+2) {
        printf("%d\n", i);

        pthread_mutex_lock(&mutex);
	even = false;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
	while (!even)
        	pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
    }
}

void* odd_thread(void* data)
{
    int limit = *(int*)data;

    for (int i = 1; i < limit; i=i+2) {
        pthread_mutex_lock(&mutex);
	while (even)
        	pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);

        printf("%d\n", i);

        pthread_mutex_lock(&mutex);
	even = true;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
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
