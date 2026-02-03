#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("Demonstrating execl vs execv\n");
    printf("=============================\n\n");
    
    // Create first child for execl
    pid_t pid1 = fork();
    
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid1 == 0) {
        // Child 1: Use execl
        printf("Child 1 (PID=%d) using execl: ", getpid());
        fflush(stdout); // Flush before exec
        
        execl("/bin/echo", "echo", "one", "two", NULL);
        
        perror("execl");
        _exit(1);
    }
    
    // Create second child for execv
    pid_t pid2 = fork();
    
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid2 == 0) {
        // Child 2: Use execv
        printf("Child 2 (PID=%d) using execv: ", getpid());
        fflush(stdout); // Flush before exec
        
        char *args[] = {"echo", "one", "two", NULL};
        execv("/bin/echo", args);
        
        perror("execv");
        _exit(1);
    }
    
    // Parent waits for both children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    printf("\nParent: Both children finished execution\n");
    return 0;
}
