#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    
    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    printf("Pipeline demo: ls | grep c\n");
    printf("=======================\n\n");
    
    // Fork first child (ls)
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid1 == 0) {
        // First child: executes 'ls -l'
        printf("[child1] PID=%d executing 'ls -l'\n", getpid());
        
        // Close read end, we only write
        close(pipefd[0]);
        
        // Redirect stdout to pipe write end
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        // Execute ls -l
        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls failed");
        _exit(1);
    }
    
    // Fork second child (grep)
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid2 == 0) {
        // Second child: executes 'grep c'
        printf("[child2] PID=%d executing 'grep c'\n", getpid());
        
        // Close write end, we only read
        close(pipefd[1]);
        
        // Redirect stdin from pipe read end
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        // Execute grep 'c' (looks for files containing 'c')
        execlp("grep", "grep", "c", NULL);
        perror("execlp grep failed");
        _exit(1);
    }
    
    // Parent process
    // Close both ends of pipe (children are using them)
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Wait for both children to finish
    printf("[parent] Waiting for children to finish...\n");
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    printf("\n[parent] Pipeline completed successfully!\n");
    return 0;
}