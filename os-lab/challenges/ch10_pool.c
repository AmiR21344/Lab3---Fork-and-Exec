#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_CONCURRENCY 10

void worker_process(const char *task, int worker_id) {
    printf("Worker %d (PID=%d): Processing task '%s'\n", 
           worker_id, getpid(), task);
    
    // Simulate work - sleep based on task number or random
    int sleep_time = 1 + (worker_id % 3);
    sleep(sleep_time);
    
    printf("Worker %d (PID=%d): Finished task '%s' after %d seconds\n", 
           worker_id, getpid(), task, sleep_time);
    
    _exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <concurrency_limit> <task1> <task2> ...\n", argv[0]);
        fprintf(stderr, "Example: %s 3 file1 file2 file3 file4 file5\n", argv[0]);
        return 1;
    }
    
    int concurrency_limit = atoi(argv[1]);
    if (concurrency_limit <= 0 || concurrency_limit > MAX_CONCURRENCY) {
        fprintf(stderr, "Error: concurrency limit must be 1-%d\n", MAX_CONCURRENCY);
        return 1;
    }
    
    int total_tasks = argc - 2;
    char *tasks[MAX_TASKS];
    
    printf("Parallel Worker Pool Demo\n");
    printf("=========================\n");
    printf("Concurrency limit: %d\n", concurrency_limit);
    printf("Total tasks: %d\n", total_tasks);
    printf("\n");
    
    // Copy tasks
    for (int i = 0; i < total_tasks; i++) {
        tasks[i] = argv[i + 2];
    }
    
    pid_t active_workers[MAX_CONCURRENCY];
    int worker_task_map[MAX_CONCURRENCY];
    int next_task = 0;
    int active_count = 0;
    
    printf("Starting work...\n\n");
    
    while (next_task < total_tasks || active_count > 0) {
        // Spawn new workers if we have capacity and tasks remaining
        while (active_count < concurrency_limit && next_task < total_tasks) {
            pid_t pid = fork();
            
            if (pid < 0) {
                perror("fork");
                return 1;
            }
            
            if (pid == 0) {
                // Child worker
                worker_process(tasks[next_task], next_task);
            } else {
                // Parent
                printf("Launched worker for task '%s' (PID=%d)\n", 
                       tasks[next_task], pid);
                
                // Find free slot
                for (int i = 0; i < concurrency_limit; i++) {
                    if (active_workers[i] == 0) {
                        active_workers[i] = pid;
                        worker_task_map[i] = next_task;
                        break;
                    }
                }
                
                active_count++;
                next_task++;
            }
        }
        
        // Check for finished workers
        if (active_count > 0) {
            int status;
            pid_t finished_pid = waitpid(-1, &status, WNOHANG);
            
            if (finished_pid > 0) {
                // Find which worker finished
                for (int i = 0; i < concurrency_limit; i++) {
                    if (active_workers[i] == finished_pid) {
                        printf("Worker for task '%s' (PID=%d) completed\n", 
                               tasks[worker_task_map[i]], finished_pid);
                        
                        active_workers[i] = 0;
                        active_count--;
                        break;
                    }
                }
            } else if (finished_pid == 0) {
                // No child finished yet, sleep a bit
                usleep(100000); // 0.1 second
            }
        }
    }
    
    printf("\nAll tasks completed!\n");
    return 0;
}
