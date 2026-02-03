#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pattern>\n", argv[0]);
        fprintf(stderr, "Example: %s \"\\\\.c$\"\n", argv[0]);
        return 1;
    }
    
    char *pattern = argv[1];
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    printf("Creating pipeline: ls | grep %s\n\n", pattern);
    
    // First child: ls
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid1 == 0) {
        // Child 1: ls
        close(pipefd[0]); // Close read end
        
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);
        
        execlp("ls", "ls", NULL);
        perror("execlp ls");
        _exit(1);
    }
    
    // Second child: grep
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }
    
    if (pid2 == 0) {
        // Child 2: grep
        close(pipefd[1]); // Close write end
        
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from pipe
        close(pipefd[0]);
        
        // Build grep command with pattern
        execlp("grep", "grep", pattern, NULL);
        perror("execlp grep");
        _exit(1);
    }
    
    // Parent closes both ends of pipe
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Wait for both children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    printf("\nPipeline completed\n");
    return 0;
}
