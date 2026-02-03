#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {
        // Child process - reads from pipe and calculates sum
        close(pipefd[1]); // Close write end
        
        int sum = 0;
        int number;
        
        // Read numbers until pipe is closed
        while (read(pipefd[0], &number, sizeof(number)) > 0) {
            printf("Child received: %d\n", number);
            sum += number;
        }
        
        close(pipefd[0]);
        
        printf("Sum = %d\n", sum);
        _exit(0);
    } else {
        // Parent process - writes numbers to pipe
        close(pipefd[0]); // Close read end
        
        // Write numbers 1 through 10
        for (int i = 1; i <= 10; i++) {
            printf("Parent sending: %d\n", i);
            write(pipefd[1], &i, sizeof(i));
        }
        
        close(pipefd[1]); // Close write end to signal EOF
        
        waitpid(pid, NULL, 0);
        printf("Parent: child finished\n");
    }
    
    return 0;
}
