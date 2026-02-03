#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void demonstrate_zombie() {
    printf("\n=== DEMONSTRATING ZOMBIE PROCESS ===\n");
    printf("A zombie process occurs when a child exits but parent hasn't called wait()\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child process
        printf("[child] PID=%d exiting immediately\n", getpid());
        printf("[child] I will become a zombie for 10 seconds\n");
        _exit(0);  // Child exits immediately
    } else {
        // Parent process
        printf("[parent] PID=%d created child PID=%d\n", getpid(), pid);
        printf("[parent] Sleeping for 10 seconds WITHOUT calling wait()\n");
        printf("[parent] During this time, child will be a zombie\n");
        printf("\nIN A SEPARATE TERMINAL, RUN: ps -l | grep %d\n", pid);
        printf("You should see 'Z' (zombie) in the STAT column\n");
        
        sleep(10);  // Parent sleeps, child is zombie
        
        // Now wait for child
        printf("\n[parent] Now calling wait() to reap zombie...\n");
        wait(NULL);
        printf("[parent] Zombie reaped!\n");
    }
}

void demonstrate_orphan() {
    printf("\n\n=== DEMONSTRATING ORPHAN PROCESS ===\n");
    printf("An orphan process occurs when parent exits before child\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child process
        printf("[child] PID=%d, PPID=%d\n", getpid(), getppid());
        printf("[child] Parent will exit in 2 seconds, making me an orphan\n");
        printf("[child] Sleeping 5 seconds...\n");
        
        sleep(2);  // Give parent time to exit
        
        // Now parent should be dead
        printf("[child] My PPID is now %d (should be 1 - init/systemd)\n", getppid());
        printf("[child] I'm now an orphan, adopted by init\n");
        
        sleep(3);  // Stay alive to demonstrate
        printf("[child] Orphan exiting\n");
        _exit(0);
    } else {
        // Parent process
        printf("[parent] PID=%d created child PID=%d\n", getpid(), pid);
        printf("[parent] Exiting in 2 seconds (before child finishes)\n");
        sleep(2);
        printf("[parent] Exiting now - child will become orphan\n");
        // Parent exits without waiting
    }
}

void prevent_zombie() {
    printf("\n\n=== PREVENTING ZOMBIE PROCESS ===\n");
    printf("Properly calling wait() immediately prevents zombies\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child
        printf("[child] PID=%d doing some work...\n", getpid());
        sleep(2);
        printf("[child] Exiting normally\n");
        _exit(0);
    } else {
        // Parent
        printf("[parent] PID=%d created child PID=%d\n", getpid(), pid);
        printf("[parent] Calling wait() immediately...\n");
        
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("[parent] Child exited normally with status %d\n", WEXITSTATUS(status));
        }
        printf("[parent] No zombie created!\n");
    }
}

int main() {
    printf("Process States Demonstration\n");
    printf("============================\n");
    
    demonstrate_zombie();
    demonstrate_orphan();
    
    // Fork again for prevention demo (orphan demo's parent already exited)
    pid_t pid = fork();
    if (pid == 0) {
        prevent_zombie();
        _exit(0);
    } else if (pid > 0) {
        wait(NULL);
    }
    
    printf("\n\nSUMMARY:\n");
    printf("- Zombie: Child exits, parent hasn't called wait()\n");
    printf("- Orphan: Parent exits, child still running\n");
    printf("- Prevent zombies: Always call wait() or use SIGCHLD handler\n");
    
    return 0;
}