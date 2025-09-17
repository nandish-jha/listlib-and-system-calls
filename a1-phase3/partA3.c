/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <partA.h>
#include <square.h>

volatile int keep_running = 1;
volatile int *square_counter = NULL;
__thread int current_thread = -1;

void *ChildThreadFunction(void *arg)
{
    int *params = (int*)arg;
    int incoming_thread_id = params[0];
    int incoming_size = params[1];

    int thread_index = incoming_thread_id - 1;
    int i;

    struct timeval child_start_time, child_end_time;
    double child_elapsed_time;

    gettimeofday(&child_start_time, NULL);
    
    printf("      Thread %d: starting now\n", incoming_thread_id);

    current_thread = thread_index;
    sleep(1);

    for (i = 1; i <= incoming_size; i++)
    {
        
        if (keep_running)
        {
            Square(i);
        }
        else if (!keep_running)
        {
            printf("   Thread %d stopped at Square(%d)\n",
                incoming_thread_id, i);
            break;
        }
    }

    gettimeofday(&child_end_time, NULL);

    /* parent elapsed time */
    child_elapsed_time = (double)(child_end_time.tv_sec - 
                         child_start_time.tv_sec) * 1000.0;
    child_elapsed_time += (child_end_time.tv_usec - 
                          child_start_time.tv_usec) / 1000.0;
    printf("      Thread %d completed, elapsed time: %.2f ms, "
           "square calls = %d\n",
           incoming_thread_id, child_elapsed_time, 
           square_counter[thread_index]);
    return NULL;
}

void *ParentThreadFunction(void *arg)
{
    pthread_t *child_thread;
    int create_child_r;

    int *param = (int*)arg;
    int number_of_threads = param[0];
    int deadline = param[1];
    int size = param[2];

    int *thread_ids;
    int (*args)[2];
    int i;

    struct timeval parent_start_time, parent_end_time;
    double parent_elapsed_time;

    gettimeofday(&parent_start_time, NULL);

    /* mem allocs */
    child_thread = (pthread_t*)malloc(number_of_threads * sizeof(pthread_t));
    thread_ids = (int*)malloc(number_of_threads * sizeof(int));
    square_counter = (int*)calloc(number_of_threads, sizeof(int));
    args = malloc(number_of_threads * sizeof(int[2]));

    if (child_thread == NULL || thread_ids == NULL || 
        square_counter == NULL || args == NULL)
    {
        printf("Memory Allocation failed\n");
        exit(1);
    }

    printf("   Parent thread started creating children\n");

    for (i = 0; i < number_of_threads; i++)
    {
        thread_ids[i] = i + 1;

        args[i][0] = thread_ids[i];
        args[i][1] = size;

        create_child_r = pthread_create(
            &child_thread[i],
            NULL,
            ChildThreadFunction,
            &args[i]);

        if (create_child_r)
        {
            printf("Failed to create child thread %d\n", i + 1);
            exit(1);
        }
        else
        {
            printf("   Child thread %d created successfully\n",
                thread_ids[i]);
        }
    }

    printf("   Parent sleeping for %d seconds\n", deadline);
    sleep(deadline);
    keep_running = 0;

    for (i = 0; i < number_of_threads; i++)
    {
        pthread_detach(child_thread[i]);
        printf("   Child thread %d closed succesfully\n", thread_ids[i]);
    }

    printf("All child threads closed\n");
    printf("Parent Summary of square calls:\n");
    for (i = 0; i < number_of_threads; i ++)
    {
        printf("   Thread %d: %d square calls\n", 
               thread_ids[i], square_counter[i]);
    }

    free(child_thread);
    free(thread_ids);
    free((void*)square_counter);
    free(args);

    gettimeofday(&parent_end_time, NULL);

    /* parent elapsed time */
    parent_elapsed_time = (double)(parent_end_time.tv_sec - 
                          parent_start_time.tv_sec) * 1000.0;
    parent_elapsed_time += (parent_end_time.tv_usec - 
                           parent_start_time.tv_usec) / 1000.0;
    printf("Parent thread completed in %.2f ms (%.2f seconds)\n",
        parent_elapsed_time, parent_elapsed_time / 1000.0);
    return NULL;    
}

int main(int argc, char *argv[])
{
    pthread_t parent_thread;
    int creat_parent_r;

    int number_of_threads;
    int deadline;
    int size;

    int input_args[3]; /* To send an array of inputs */

    struct timeval start_time, end_time;
    double total_elapsed_time;

    if (argc != 4)
    {
        printf("Error: Expecting 3 arguments\n");
        return 1;
    }
    else
    {
        number_of_threads = strtol(argv[1], NULL, 0);
        deadline = strtol(argv[2], NULL, 0);
        size = strtol(argv[3], NULL, 0);

        if (number_of_threads <= 0 || number_of_threads > 1000)
        {
            printf("Error: Number of threads must be between 1 and 1000\n");
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
        printf("  Number of Threads: %d\n", number_of_threads);
        printf("  Deadline (seconds): %d\n", deadline);
        printf("  Size for Square(): %d\n", size);
        printf("\n");
    }

    gettimeofday(&start_time, NULL);

    input_args[0] = number_of_threads;
    input_args[1] = deadline;
    input_args[2] = size;

    /* create parent thread */
    creat_parent_r = pthread_create(
        &parent_thread,
        NULL,
        ParentThreadFunction,
        input_args);

    /* syscall for thread creation returns 0 for succes */
    if (creat_parent_r)
    {
        printf("Failed to create parent thread\n");
        return 1;
    }
    else
    {
        printf("Parent thread created successfully\n");
    }

    pthread_detach(parent_thread); /* fire and forget behaviour with detach */
    sleep(deadline + 1);
    printf("Parent thread closed\n");

    gettimeofday(&end_time, NULL);

    /* eleapsed time */
    total_elapsed_time = (double)(end_time.tv_sec -
        start_time.tv_sec) * 1000.0;
    total_elapsed_time += (end_time.tv_usec - 
                          start_time.tv_usec) / 1000.0;
    printf("\n=== PROGRAM SUMMARY ===\n");
    printf("   Total program elepsed times: %.2f ms (%.2f seconds)\n\n",
        total_elapsed_time, total_elapsed_time / 1000.0);

    return 0;
}
