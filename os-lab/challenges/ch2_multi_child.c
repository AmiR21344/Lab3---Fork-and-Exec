#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_children>\n", argv[0]);
        return 1;
    }
    
    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Error: N must be positive\n");
        return 1;
    }
    
    printf("Parent (PID=%d) creating %d children...\n", getpid(), N);
    
    pid_t children[N];
    
    // Create N children
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        
        if (pid == 0) {
            // Child process
            printf("Child %d (PID=%d): Hello!\n", i, getpid());
            sleep(i + 1); // Different sleep times to show order
            printf("Child %d (PID=%d): Exiting with code %d\n", i, getpid(), i + 1);
            _exit(i + 1);
        } else {
            // Parent remembers child PID
            children[i] = pid;
        }
    }
    
    // Parent waits for all children
    printf("\nParent waiting for all children...\n");
    
    for (int i = 0; i < N; i++) {
        int status;
        pid_t finished_pid = wait(&status);
        
        if (finished_pid == -1) {
            perror("wait");
            continue;
        }
        
        // Find which child this was
        int child_index = -1;
        for (int j = 0; j < N; j++) {
            if (children[j] == finished_pid) {
                child_index = j;
                break;
            }
        }
        
        if (WIFEXITED(status)) {
            printf("Parent: child %d (PID=%d) finished with status %d\n", 
                   child_index, finished_pid, WEXITSTATUS(status));
        } else {
            printf("Parent: child %d (PID=%d) did not exit normally\n", 
                   child_index, finished_pid);
        }
    }
    
    printf("All children finished!\n");
    return 0;
}
