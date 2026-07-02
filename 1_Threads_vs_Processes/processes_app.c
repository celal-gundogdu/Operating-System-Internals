#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 600000
#define RANGE      200

int arr[ARRAY_SIZE];

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_processes>\n", argv[0]);
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


    int (*pipes)[2] = malloc(N * sizeof(int[2]));
    for (int i = 0; i < N; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    int chunk    = ARRAY_SIZE / N;
    int leftover = ARRAY_SIZE % N;

    for (int i = 0; i < N; i++) {

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {

            for (int j = 0; j < N; j++) {
                if (j == i) {
                    close(pipes[j][0]); 
                } else {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            int start = i * chunk;
            int end   = (i == N - 1) ? (start + chunk + leftover - 1)
                                      : (start + chunk - 1);

            long long sum = 0;
            for (int k = start; k <= end; k++) {
                sum += arr[k];
            }
            int count = end - start + 1;
            double sub_avg = (double)sum / count;

            write(pipes[i][1], &sub_avg, sizeof(double));
            close(pipes[i][1]);

            exit(0);
        }

        close(pipes[i][1]);
    }

    double total_avg = 0.0;

    for (int i = 0; i < N; i++) {
        wait(NULL);

        double sub_avg = 0.0;
        read(pipes[i][0], &sub_avg, sizeof(double));
        close(pipes[i][0]);

        total_avg += sub_avg;
    }

    total_avg /= N;

    printf("Array size : %d\n", ARRAY_SIZE);
    printf("Processes  : %d\n", N);
    printf("Final avg  : %.4f\n", total_avg);

    free(pipes);
    
    return 0;
}
