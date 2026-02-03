#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    
    // Create a pipe
    if (pipe(fd) == -1) { 
        perror("pipe"); 
        exit(1); 
    }

    pid_t pid = fork();
    if (pid < 0) { 
        perror("fork"); 
        exit(1); 
    }

    if (pid == 0) {
        // Child: read from pipe and run wc -l on stdin
        close(fd[1]);  // Close write end
        
        // Redirect stdin to read from pipe
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);  // Close original read end
        
        // Execute wc -l (will read from stdin)
        execlp("wc", "wc", "-l", (char *)NULL);
        
        // If we get here, exec failed
        perror("execlp wc");
        _exit(1);
    } else {
        // Parent: write lines to pipe
        close(fd[0]);  // Close read end
        
        FILE *out = fdopen(fd[1], "w");
        
        // Write 5 lines to the pipe
        for (int i = 0; i < 5; ++i) {
            fprintf(out, "line %d\n", i);
        }
        
        fclose(out); // This closes fd[1]
        
        // Wait for child to finish
        waitpid(pid, NULL, 0);
    }

    return 0;
}