#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("Worker (PID=%d):\n", getpid());
    printf("Arguments received:\n");
    
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] = %s\n", i, argv[i]);
    }
    
    // Look for MYVAR in environment
    char *myvar = NULL;
    for (int i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], "MYVAR=", 6) == 0) {
            myvar = envp[i] + 6; // Skip "MYVAR="
            break;
        }
    }
    
    if (myvar) {
        printf("MYVAR = %s\n", myvar);
    } else {
        printf("MYVAR is not set\n");
    }
    
    return 0;
}
