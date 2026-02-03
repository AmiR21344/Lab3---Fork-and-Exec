#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {
        // Child process
        printf("Hello from child (PID=%d, PPID=%d)\n", getpid(), getppid());
        sleep(2);
        printf("Child exiting with code 7\n");
        _exit(7);
    } else {
        // Parent process
        printf("Parent (PID=%d) created child (PID=%d)\n", getpid(), pid);
        
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Parent: child %d exited with status %d\n", pid, WEXITSTATUS(status));
        } else {
            printf("Parent: child %d did not exit normally\n", pid);
        }
    }
    
    return 0;
}
