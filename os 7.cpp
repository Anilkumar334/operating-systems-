#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

// Structure to represent shared data
struct SharedData {
    int counter;
};

int main() {
    int shmid;
    struct SharedData *shared_data;

    // Create a key for shared memory
    key_t key = ftok("shared_memory_example", 'R');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create or open shared memory segment
    shmid = shmget(key, sizeof(struct SharedData), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory
    shared_data = (struct SharedData *)shmat(shmid, NULL, 0);
    if ((intptr_t)shared_data == -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize the counter
    shared_data->counter = 0;

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        sleep(2); // Simulating some work

        // Child increments the counter in shared memory
        shared_data->counter += 1;

        printf("Child Process: Counter = %d\n", shared_data->counter);

        // Detach from the shared memory
        if (shmdt(shared_data) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

    } else { // Parent process
        // Wait for the child to finish
        wait(NULL);

        // Parent reads and displays the updated counter from shared memory
        printf("Parent Process: Counter = %d\n", shared_data->counter);

        // Detach from the shared memory
        if (shmdt(shared_data) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        // Remove the shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

