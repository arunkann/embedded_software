#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>

#define E2O_SEM "/e2o_sem"
#define O2E_SEM "/o2e_sem"

/* Semaphore is an integer whose value never falls below 0.
 * sem_post increments semaphore.
 * sem_wait decrements semaphore, if current value > 0. If 0, it blocks.
 */

void* even_process(void* data)
{
    int limit = *(int*)data;
    int status;
    int val;
    sem_t *e2o_sem, *o2e_sem;

    o2e_sem = sem_open(O2E_SEM, O_RDWR);
    e2o_sem = sem_open(E2O_SEM, O_RDWR);

    if ((o2e_sem == SEM_FAILED) || (e2o_sem == SEM_FAILED)) {
        printf("semaphore init error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < limit; i=i+2) {
        /* wait for odd process */
        status = sem_wait(o2e_sem);
        if (status)
            printf("even: sem_wait ret error %d\n", errno);

        /* Without newline, printf may not flush to screen. */
        printf("%d\n", i);

        /* done printing even; msg odd process */
        status = sem_post(e2o_sem);
        if (status)
            printf("even: sem_post ret error %d\n", errno);
    }

    sem_close(o2e_sem);
    sem_close(e2o_sem);
    exit(0);
}

void* odd_process(void* data)
{
    int limit = *(int*)data;
    int val;
    int status;
    sem_t *e2o_sem, *o2e_sem;

    o2e_sem = sem_open(O2E_SEM, O_RDWR);
    e2o_sem = sem_open(E2O_SEM, O_RDWR);

    if ((o2e_sem == SEM_FAILED) || (e2o_sem == SEM_FAILED)) {
        printf("semaphore init error\n");
        exit(EXIT_FAILURE);
    }

    /* this post kicks off even process to start printing */
    status = sem_post(o2e_sem);
    if (status)
        printf("odd: sem_post ret error %d\n", errno);

    for (int i = 1; i < limit; i=i+2) {
        /* wait for even process */
        status = sem_wait(e2o_sem);
        if (status)
            printf("odd: sem_wait ret error %d\n", errno);

        /* Without newline, printf may not flush to screen. */
        printf("%d\n", i);

        /* done printing odd; msg even process */
        status = sem_post(o2e_sem);
        if (status)
            printf("odd: sem_post ret error %d\n", errno);
    }

    sem_close(o2e_sem);
    sem_close(e2o_sem);
    exit(0);
}

int main()
{
    int limit = 20;
    pid_t parent_pid, odd_pid, even_pid;
    int status;
    int val;
    sem_t *e2o_sem, *o2e_sem;
    int rc;

    o2e_sem = sem_open(O2E_SEM, O_CREAT, 0644, 0);
    e2o_sem = sem_open(E2O_SEM, O_CREAT, 0644, 0);

    if ((o2e_sem == SEM_FAILED) || (e2o_sem == SEM_FAILED)) {
        printf("semaphore init error\n");
        exit(EXIT_FAILURE);
    }

#if 0
    /* Just sanity check */
    sem_getvalue(o2e_sem, &val);
    printf("o2e_sem value %d\n", val);
    sem_getvalue(e2o_sem, &val);
    printf("e2o_sem value %d\n", val);
#endif

    /* save parent pid to distinguish from child */
    parent_pid = getpid();

    /* Upon fork(), child is returned pid 0 and parent is returned non-zero pid */
    if ((parent_pid == getpid()) && ((even_pid = fork()) == 0))
        even_process((void*) &limit);
    if ((parent_pid == getpid()) && ((odd_pid = fork()) == 0))
        odd_process((void*) &limit);   

    if (parent_pid == getpid() && (even_pid < 0 || odd_pid < 0))
        printf("Failed to fork process\n");

    if (parent_pid == getpid()) {
        /* wait for children to finish */
        while (wait(&status) > 0);

    	sem_close(e2o_sem);
    	sem_close(o2e_sem);
    	rc = sem_unlink(E2O_SEM);
        if (rc)
            printf("o2e sem unlink failed\n");
    	rc = sem_unlink(O2E_SEM);
        if (rc)
            printf("o2e sem unlink failed\n");
    }

    return 0;
}
