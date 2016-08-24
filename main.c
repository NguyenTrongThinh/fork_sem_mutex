#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


long number = 6;
pthread_mutex_t mutex;
sem_t sem;
int process(char* program, char** argv)
{
    pid_t child_pid;
    child_pid = fork();
    if (child_pid != 0)
    {
        return child_pid;
    }
    else
    {
        execvp(program, argv);
        fprintf(stderr, "An error occourred in exevp\n");
        abort();
    }
}
void method1()
{
    pthread_mutex_lock(&mutex);
    number *= 5;
    number /= 4;
    pthread_mutex_unlock(&mutex);
}
void method2()
{
    pthread_mutex_lock(&mutex);
    number *= 3;
    number /= 2;
    pthread_mutex_unlock(&mutex);
}
void *thread1()
{
    method1();
    sleep(1);
    printf("From thread 1: %d", number);

}
void *thread2()
{
    method2();
    sleep(2);
    printf("From thread 2: %d", number);
    fflush(stdout);
}
void *button()
{
    while(1)
    {
        sem_post(&sem);
        sleep(1);
    }
}
void *led()
{
    int sem_val = 0;
    while(1)
    {
        sem_wait(&sem);
        sem_getvalue(&sem, &sem_val);
        printf("semvalue %d...\n", sem_val);
        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    char* program="ls";
    char* argvv[] = {"-la", "/", NULL};
    pthread_t thread1_id, thread2_id, thread3_id, thread4_id;
    sem_init(&sem, 0, 1);

    pthread_create(&thread1_id, NULL, &thread1, NULL);
    pthread_create(&thread2_id, NULL, &thread2, NULL);
    pthread_create(&thread3_id, NULL, &led, NULL);
    pthread_create(&thread3_id, NULL, &button, NULL);
    pthread_join(thread1_id, NULL);
    pthread_join(thread2_id, NULL);
    //pthread_join(thread3_id, NULL);  //Neu Thread nay ket thuc chuong trinh moi ket thuc
    //pthread_join(thread4_id, NULL);
    process(program, argvv);
    sleep(10);
    sem_destroy(&sem);
    return 0;
}
