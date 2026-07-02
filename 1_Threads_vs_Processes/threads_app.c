#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 600000
#define RANGE      200


int arr[ARRAY_SIZE];


typedef struct {
    int start_idx; 
    int end_idx;   
} ThreadArgs;



void *thread_func(void *arg)
{
    ThreadArgs *targs = (ThreadArgs *)arg;
    long long sum = 0;

    for (int i = targs->start_idx; i <= targs->end_idx; i++) {
        sum += arr[i];
    }

    int count = targs->end_idx - targs->start_idx + 1;

    double *result = malloc(sizeof(double));
    *result = (double)sum / count;

    return (void *)result;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0 || N > ARRAY_SIZE) {
        fprintf(stderr, "N must be between 1 and %d\n", ARRAY_SIZE);
        return 1;
    }

    srand((unsigned int)time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = (rand() % RANGE) + 1; 
    }

    int chunk = ARRAY_SIZE / N;   
    int leftover = ARRAY_SIZE % N; 

    pthread_t   *threads = malloc(N * sizeof(pthread_t));
    ThreadArgs  *targs   = malloc(N * sizeof(ThreadArgs));

    for (int i = 0; i < N; i++) {
        targs[i].start_idx = i * chunk;
        targs[i].end_idx   = (i + 1) * chunk - 1;
    }

    targs[N - 1].end_idx += leftover;


    for (int i = 0; i < N; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, &targs[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }


    double total_avg = 0.0;
    for (int i = 0; i < N; i++) {
        void *ret;
        pthread_join(threads[i], &ret);

        double *sub_avg = (double *)ret;
        total_avg += *sub_avg;
        free(sub_avg); 
    }

    total_avg /= N;

    printf("Array size : %d\n", ARRAY_SIZE);
    printf("Threads    : %d\n", N);
    printf("Final avg  : %.4f\n", total_avg);

    free(threads);
    free(targs);
    
    return 0;
}
