#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {
        // Child process
        printf("Child (PID=%d) executing worker...\n", getpid());
        
        // Prepare arguments for worker
        char *args[] = {"worker", "arg1", "arg2", "arg3", NULL};
        
        // Prepare custom environment
        char *env[] = {
            "MYVAR=hello",
            "PATH=/bin:/usr/bin",
            "HOME=/tmp",
            NULL
        };
        
        // Execute worker with custom environment
        execle("./worker", args[0], args[1], args[2], args[3], NULL, env);
        
        // If we get here, exec failed
        perror("execle");
        _exit(1);
    } else {
        // Parent process
        printf("Parent (PID=%d) created child (PID=%d)\n", getpid(), pid);
        
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Parent: worker finished with status %d\n", WEXITSTATUS(status));
        }
    }
    
    return 0;
}
