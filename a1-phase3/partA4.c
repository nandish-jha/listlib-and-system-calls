/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <partA.h>
#include <square.h>

/* process creation (for doing it globally) */
pid_t child_process;

volatile int keep_running = 1;
volatile int *square_counter = NULL;
__thread int current_thread = -1;

/* Global variables for shared memory and processes */
pid_t *child_processes = NULL;
int *process_ids = NULL;
int shmid = -1;

struct timeval start_time, end_time;
double total_elapsed_time;

void signal_handler(int sig)
{
    keep_running = 0;
}

void ChildProcessFunction(int process_id, int size, int *shared_counter)
{
    int i;
    int process_index = process_id - 1;
    struct timeval child_start_time, child_end_time;
    double child_elapsed_time;

    signal(SIGUSR1, signal_handler);
    
    gettimeofday(&child_start_time, NULL);
    
    printf("      Process %d starting now\n", process_id);
    sleep(1);

    for (i = 1; i <= size; i++)
    {
        if (keep_running)
        {
            current_thread = process_index; /* Set before each Square() call */
            Square(i);
        }
        else
        {
            printf("   Process %d stopped at Square(%d)\n", process_id, i);
            break;
        }
    }

    gettimeofday(&child_end_time, NULL);

    child_elapsed_time = (double)(child_end_time.tv_sec - 
                         child_start_time.tv_sec) * 1000.0;
    child_elapsed_time += (child_end_time.tv_usec - 
                          child_start_time.tv_usec) / 1000.0;
    
    printf("      Process %d completed, elapsed time: %.2f ms, "
           "square calls = %d\n",
           process_id, child_elapsed_time, 
           shared_counter[process_index]);
}

int main(int argc, char *argv[])
{
    int number_of_processes;
    int deadline;
    int size;
    int i;
    struct timeval parent_start_time, parent_end_time;
    double parent_elapsed_time;

    if (argc != 4)
    {
        printf("Error: Expecting 3 arguments\n");
        return 1;
    }

    number_of_processes = strtol(argv[1], NULL, 0);
    deadline = strtol(argv[2], NULL, 0);
    size = strtol(argv[3], NULL, 0);

    if (number_of_processes <= 0 || number_of_processes > 1000)
    {
        printf("Error: Number of processes must be between 1 and 1000\n");
        return 1;
    }
    
    if (deadline <= 0 || deadline > 120)
    {
        printf("Error: Deadline must be between 1 and 120 seconds\n");
        return 1;
    }

    if (size <= 0 || size > 10000)
    {
        printf("Error: Size must be between 1 and 10000\n");
        return 1;
    }

    printf("Inputs received...\n");
    printf("  Number of Processes: %d\n", number_of_processes);
    printf("  Deadline (seconds): %d\n", deadline);
    printf("  Size for Square(): %d\n", size);
    printf("\n");

    gettimeofday(&start_time, NULL);
    gettimeofday(&parent_start_time, NULL);

    /* Create shared memory for square counters */
    shmid = shmget(IPC_PRIVATE, number_of_processes * sizeof(int), 
                   IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Failed to create shared memory");
        return 1;
    }

    square_counter = (int*)shmat(shmid, NULL, 0);
    if (square_counter == (int*)-1)
    {
        perror("Failed to attach shared memory");
        return 1;
    }

    /* Initialize shared memory */
    for (i = 0; i < number_of_processes; i++)
    {
        square_counter[i] = 0;
    }

    /* Allocate arrays for process management */
    child_processes = (pid_t*)malloc(number_of_processes * sizeof(pid_t));
    process_ids = (int*)malloc(number_of_processes * sizeof(int));

    if (child_processes == NULL || process_ids == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Parent process started creating children\n");

    /* Create child processes */
    for (i = 0; i < number_of_processes; i++)
    {
        process_ids[i] = i + 1;
        
        child_processes[i] = fork();
        
        if (child_processes[i] == -1)
        {
            perror("Failed to create child process");
            return 1;
        }
        else if (child_processes[i] == 0)
        {
            /* Child process */
            ChildProcessFunction(process_ids[i], size, (int*)square_counter);
            exit(0);
        }
        else
        {
            /* Parent process */
            printf("   Child process created successfully (PID: %d)\n", 
                   process_ids[i]);
        }
    }

    /* Parent sleeps for deadline */
    printf("   Parent sleeping for %d seconds\n", deadline);
    sleep(deadline);
    keep_running = 0;

    /* Send termination signal to all children */
    for (i = 0; i < number_of_processes; i++)
    {
        kill(child_processes[i], SIGUSR1);
    }

    /* Wait for all children to complete */
    for (i = 0; i < number_of_processes; i++)
    {
        waitpid(child_processes[i], NULL, 0);
        printf("   Child process %d closed successfully\n", process_ids[i]);
    }

    printf("All child processes closed\n");
    printf("Parent Summary of square calls:\n");
    for (i = 0; i < number_of_processes; i++)
    {
        printf("   Process %d: %d square calls\n", 
               process_ids[i], square_counter[i]);
    }

    gettimeofday(&parent_end_time, NULL);

    parent_elapsed_time = (double)(parent_end_time.tv_sec - 
                          parent_start_time.tv_sec) * 1000.0;
    parent_elapsed_time += (parent_end_time.tv_usec - 
                           parent_start_time.tv_usec) / 1000.0;
    printf("Parent process completed in %.2f ms (%.2f seconds)\n",
        parent_elapsed_time, parent_elapsed_time / 1000.0);

    gettimeofday(&end_time, NULL);

    total_elapsed_time = (double)(end_time.tv_sec - start_time.tv_sec) * 1000.0;
    total_elapsed_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;
    
    printf("\n=== PROGRAM SUMMARY ===\n");
    printf("   Total program elapsed time: %.2f ms (%.2f seconds)\n\n",
        total_elapsed_time, total_elapsed_time / 1000.0);

    /* Cleanup */
    free(child_processes);
    free(process_ids);
    shmdt((void*)square_counter);  /* Cast to remove volatile qualifier */
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
