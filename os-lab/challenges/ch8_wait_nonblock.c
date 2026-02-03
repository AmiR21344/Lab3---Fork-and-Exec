#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    printf("Non-blocking wait demonstration\n");
    printf("===============================\n\n");
    
    pid_t children[3];
    int sleep_times[3] = {1, 2, 3};
    
    // Create 3 children
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        
        if (pid == 0) {
            // Child process
            printf("Child %d (PID=%d): Sleeping for %d seconds\n", 
                   i, getpid(), sleep_times[i]);
            
            sleep(sleep_times[i]);
            
            printf("Child %d (PID=%d): Exiting\n", i, getpid());
            _exit(i);
        } else {
            // Parent remembers child
            children[i] = pid;
            printf("Parent created child %d (PID=%d)\n", i, pid);
        }
    }
    
    printf("\nParent (PID=%d) starting non-blocking wait loop...\n", getpid());
    
    int children_done = 0;
    time_t start_time = time(NULL);
    
    while (children_done < 3) {
        int status;
        pid_t finished_pid = waitpid(-1, &status, WNOHANG);
        
        if (finished_pid > 0) {
            // A child finished
            for (int i = 0; i < 3; i++) {
                if (children[i] == finished_pid) {
                    if (WIFEXITED(status)) {
                        printf("[%.0fs] Parent: child %d (PID=%d) finished with status %d\n",
                               difftime(time(NULL), start_time), i, finished_pid, WEXITSTATUS(status));
                    }
                    children_done++;
                    break;
                }
            }
        } else if (finished_pid == 0) {
            // No child finished yet
            printf("[%.0fs] Parent: checking... (no child finished yet)\n", 
                   difftime(time(NULL), start_time));
            sleep(1); // Sleep a bit to avoid busy-waiting
        } else {
            // Error
            perror("waitpid");
            break;
        }
    }
    
    printf("\nAll children finished!\n");
    return 0;
}
