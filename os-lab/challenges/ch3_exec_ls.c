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
        printf("Child (PID=%d) executing 'ls -la'...\n\n", getpid());
        
        // Note: There's a typo in the challenge - should be execlp, not exec1p
        execlp("ls", "ls", "-la", NULL);
        
        // If we get here, exec failed
        perror("execlp");
        _exit(1);
    } else {
        // Parent process
        printf("Parent (PID=%d) created child (PID=%d)\n", getpid(), pid);
        
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("\nParent: child finished with status %d\n", WEXITSTATUS(status));
        } else {
            printf("\nParent: child did not exit normally\n");
        }
    }
    
    return 0;
}
