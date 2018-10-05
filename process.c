#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>

#define E2O_SEM "/e2o_sem"
#define O2E_SEM "/o2e_sem"

void* even_process(void* data)
{
    int limit = *(int*)data;
    int status;
    int val;
    sem_t *e2o_sem, *o2e_sem;

    o2e_sem = sem_open(O2E_SEM, O_RDWR, 0644, 0);
    e2o_sem = sem_open(E2O_SEM, O_RDWR, 0644, 0);

    if ((o2e_sem == SEM_FAILED) || (e2o_sem == SEM_FAILED)) {
        printf("semaphore init error\n");
        exit(EXIT_FAILURE);
    }

    /* wait for odd process to start */
    status = sem_wait(o2e_sem);
    if (status)
        printf("even: sem_wait ret error %d\n", errno);

    /* hand off to odd process to start for loop prints */
    status = sem_post(e2o_sem);
    if (status)
        printf("even: sem_post ret error %d\n", errno);

    for (volatile int i = 0; i < limit; i=i+2) {
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
}

void* odd_process(void* data)
{
    int limit = *(int*)data;
    int val;
    int status;
    sem_t *e2o_sem, *o2e_sem;

    o2e_sem = sem_open(O2E_SEM, O_RDWR, 0644, 0);
    e2o_sem = sem_open(E2O_SEM, O_RDWR, 0644, 0);

    if ((o2e_sem == SEM_FAILED) || (e2o_sem == SEM_FAILED)) {
        printf("semaphore init error\n");
        exit(EXIT_FAILURE);
    }

    /* signal even process */
    status = sem_post(o2e_sem);
    if (status)
        printf("odd: sem_post ret error %d\n", errno);

    /* get control back to start for loop */
    status = sem_wait(e2o_sem);
    if (status)
        printf("odd: sem_wait ret error %d\n", errno);

    /* this post kicks off even process to start printing */
    status = sem_post(o2e_sem);
    if (status)
        printf("odd: sem_post ret error %d\n", errno);

    for (volatile int i = 1; i < limit; i=i+2) {
        /* wait for even process */
        status = sem_getvalue(o2e_sem, &val);
        if (status)
            printf("odd: sem_getvalue ret error %d\n", errno);

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
}

int main()
{
    int limit = 20;
    pid_t parent_pid, odd_pid, even_pid;
    int status;
    int val;
    sem_t *e2o_sem, *o2e_sem;

    o2e_sem = sem_open(O2E_SEM, O_CREAT, 0644, 0);
    e2o_sem = sem_open(E2O_SEM, O_CREAT, 0644, 0);

    if ((o2e_sem == SEM_FAILED) || (e2o_sem == SEM_FAILED)) {
        printf("semaphore init error\n");
        exit(EXIT_FAILURE);
    }

    /* Ensure initial value is 0 */
    sem_getvalue(o2e_sem, &val);
    for (int i=0; i<val; i++)
        sem_wait(o2e_sem);
    sem_getvalue(o2e_sem, &val);

    sem_getvalue(e2o_sem, &val);
    for (int i=0; i<val; i++)
        sem_wait(e2o_sem);
    sem_getvalue(e2o_sem, &val);

    /* save parent pid to distinguish from child */
    parent_pid = getpid();

    /* Upon fork(), child is returned pid 0 and parent is returned non-zero pid */
    if ((parent_pid == getpid()) && ((even_pid = fork()) == 0))
        even_process((void*) &limit);
    if ((parent_pid == getpid()) && ((odd_pid = fork()) == 0))
        odd_process((void*) &limit);   

    if (parent_pid == getpid() && (even_pid < 0 || odd_pid < 0))
        printf("Failed to fork process\n");

    /* wait for children to finish */
    if (parent_pid == getpid())
        while (wait(&status) > 0);

    if (parent_pid == getpid()) {
    	sem_close(e2o_sem);
    	sem_close(o2e_sem);
    	sem_unlink("/odd_to_even_sem");
    	sem_unlink("/even_to_odd_sem");
    }

    return 0;
}
