#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[NUM_PHILOSOPHERS];
bool fork_available[NUM_PHILOSOPHERS] = {true, true, true, true, true};

void *philosopher(void *num)
{
    int id = *(int *)num;

    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    while (1)
    {
        printf("Philosopher %d is thinking.\n", id);
        usleep(10000); // Sleep for 0.01 seconds

        pthread_mutex_lock(&mutex);

        // wait two forks to be available
        while (!fork_available[left_fork] || !fork_available[right_fork])
        {
            pthread_cond_wait(&cond[id], &mutex);
        }

        // pick up forks
        fork_available[left_fork] = false;
        fork_available[right_fork] = false;

        printf("Philosopher %d picked up left fork %d and right fork %d and starts eating.\n", id, left_fork, right_fork);

        pthread_mutex_unlock(&mutex);

        usleep(10000); // eat for 0.01 seconds

        pthread_mutex_lock(&mutex);

        // put down forks
        fork_available[left_fork] = true;
        fork_available[right_fork] = true;
        printf("Philosopher %d put down left fork %d and right fork %d.\n", id, left_fork, right_fork);

        // signal other philosophers waiting for forks
        pthread_cond_signal(&cond[left_fork]);
        pthread_cond_signal(&cond[right_fork]);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main()
{
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_numbers[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_cond_init(&cond[i], NULL);
        philosopher_numbers[i] = i;
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_cond_destroy(&cond[i]);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}