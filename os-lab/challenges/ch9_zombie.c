#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("Zombie Process Demonstration\n");
    printf("============================\n\n");
    
    int immediate_wait = 0;
    
    if (argc > 1 && strcmp(argv[1], "nowait") == 0) {
        immediate_wait = 1;
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {
        // Child process
        printf("Child (PID=%d): Exiting immediately\n", getpid());
        _exit(42);
    } else {
        // Parent process
        printf("Parent (PID=%d): Created child (PID=%d)\n", getpid(), pid);
        
        if (immediate_wait) {
            printf("Parent: Waiting immediately (no zombie)\n");
            int status;
            waitpid(pid, &status, 0);
            printf("Parent: Child reaped, status = %d\n", WEXITSTATUS(status));
        } else {
            printf("\n=== ZOMBIE MODE ===\n");
            printf("Parent: Sleeping for 10 seconds WITHOUT wait()\n");
            printf("Child is now a ZOMBIE (check in another terminal):\n");
            printf("  ps -l | grep %d\n", pid);
            printf("  Look for 'Z' in the STAT column\n");
            printf("\nWaiting 10 seconds...\n");
            
            for (int i = 1; i <= 10; i++) {
                printf("%d... ", i);
                fflush(stdout);
                sleep(1);
            }
            
            printf("\n\nParent: Now calling wait() to reap zombie...\n");
            int status;
            waitpid(pid, &status, 0);
            printf("Parent: Zombie reaped, status = %d\n", WEXITSTATUS(status));
        }
    }
    
    printf("\nInstructions for observation:\n");
    printf("1. Run without argument to see zombie: %s\n", argv[0]);
    printf("2. Run with 'nowait' to prevent zombie: %s nowait\n", argv[0]);
    printf("3. Check in another terminal: ps -l | grep Z\n");
    
    return 0;
}
