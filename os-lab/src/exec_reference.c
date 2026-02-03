#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void demo_execl() {
    printf("\n1. execl (list arguments):\n");
    if (fork() == 0) {
        // execl requires full path
        execl("/bin/echo", "echo", "Hello from", "execl", NULL);
        perror("execl failed");
        _exit(1);
    }
    wait(NULL);
}

void demo_execlp() {
    printf("\n2. execlp (search PATH):\n");
    if (fork() == 0) {
        // execlp searches PATH for 'date'
        execlp("date", "date", "+%Y-%m-%d %H:%M:%S", NULL);
        perror("execlp failed");
        _exit(1);
    }
    wait(NULL);
}

void demo_execv() {
    printf("\n3. execv (vector/array arguments):\n");
    if (fork() == 0) {
        char *args[] = {"/bin/ls", "-la", ".", NULL};
        execv("/bin/ls", args);
        perror("execv failed");
        _exit(1);
    }
    wait(NULL);
}

void demo_execvp() {
    printf("\n4. execvp (vector + PATH search):\n");
    if (fork() == 0) {
        char *args[] = {"ps", "aux", NULL};
        execvp("ps", args);
        perror("execvp failed");
        _exit(1);
    }
    wait(NULL);
}

void demo_execle() {
    printf("\n5. execle (with environment):\n");
    if (fork() == 0) {
        char *env[] = {"MYVAR=CustomValue", "PATH=/bin:/usr/bin", NULL};
        execle("/bin/sh", "sh", "-c", "echo $MYVAR && echo $PATH", NULL, env);
        perror("execle failed");
        _exit(1);
    }
    wait(NULL);
}

void demo_execve() {
    printf("\n6. execve (kernel-level with env):\n");
    if (fork() == 0) {
        char *args[] = {"printenv", NULL};
        char *env[] = {"CUSTOM_ENV=HelloWorld", "PATH=/usr/bin", NULL};
        execve("/usr/bin/printenv", args, env);
        perror("execve failed");
        _exit(1);
    }
    wait(NULL);
}

int main() {
    printf("exec() Family Function Reference\n");
    printf("================================\n");
    
    demo_execl();
    demo_execlp();
    demo_execv();
    demo_execvp();
    demo_execle();
    demo_execve();
    
    printf("\n\nSummary of exec() functions:\n");
    printf("  l = list arguments (execl, execlp, execle)\n");
    printf("  v = vector/array arguments (execv, execvp, execve)\n");
    printf("  p = searches PATH (execlp, execvp)\n");
    printf("  e = custom environment (execle, execve)\n");
    
    return 0;
}
