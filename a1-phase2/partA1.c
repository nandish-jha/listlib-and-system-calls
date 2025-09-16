/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <partA.h>
#include <square.h>

#define RED     "\033[0;31m"
#define CYAN    "\033[0;36m"
#define GREEN   "\033[0;32m"
#define WHITE   "\033[0;37m"
#define YELLOW  "\033[0;33m"

/* Global flag checked by the child threads */
volatile int keep_running = 1;
volatile int *square_counter = NULL;
__thread int current_thread = -1;

DWORD WINAPI ChildThreadFunction(LPVOID lpParam)
{
    int *params = (int*)lpParam;
    int incoming_thread_id = params[0];
    int incoming_size = params[1];

    int thread_index = incoming_thread_id - 1;
    int i;

    SYSTEMTIME start_time, end_time;
    FILETIME start_ft, end_ft;
    ULARGE_INTEGER start_uli, end_uli;
    double elapsed_ms;

    printf("         Thread %d: Starting now\n",
        incoming_thread_id);

    /* start times */
    GetSystemTime(&start_time);
    SystemTimeToFileTime(&start_time, &start_ft);
    start_uli.LowPart = start_ft.dwLowDateTime;
    start_uli.HighPart = start_ft.dwHighDateTime;

    /* Local variable giving its value to global variable*/
    current_thread = thread_index;
    Sleep(1);

    for (i = 1; i <= incoming_size; i++)
    {
        if (keep_running)
        {
            Square(i);
        }
        else if (!keep_running)
        {
            printf(RED "   Thread %d stopped at Square(%d)\n" WHITE,
                incoming_thread_id, i);
            break;
        }
    }

    /* end times */
    GetSystemTime(&end_time);
    SystemTimeToFileTime(&end_time, &end_ft);
    end_uli.LowPart = end_ft.dwLowDateTime;
    end_uli.HighPart = end_ft.dwHighDateTime;

    /* elapsed time */
    elapsed_ms = (double)(end_uli.QuadPart - start_uli.QuadPart) / 10000.0;

    printf("      Thread %d done, elapsed time: %.2f ms, square calls = %d\n",
        incoming_thread_id, elapsed_ms, square_counter[thread_index]);

    return 0;
}

DWORD WINAPI ParentThreadFunction(LPVOID lpParam)
{
    HANDLE *h_child_thread;
    /* DWORD child_thread_id; */
    int *params = (int*)lpParam;
    int number_of_threads = params[0];
    int deadline = params[1];
    int size = params[2];

    int *thread_ids;
    int (*args)[2];
    int i;

    SYSTEMTIME parent_start_time, parent_end_time;
    FILETIME parent_start_ft, parent_end_ft;
    ULARGE_INTEGER parent_start_uli, parent_end_uli;
    double parent_elapsed_ms;
    
    /* parent start time */
    GetSystemTime(&parent_start_time);
    SystemTimeToFileTime(&parent_start_time, &parent_start_ft);
    parent_start_uli.LowPart = parent_start_ft.dwLowDateTime;
    parent_start_uli.HighPart = parent_start_ft.dwHighDateTime;

    /* mem allocs */
    h_child_thread = (HANDLE*)malloc(number_of_threads * sizeof(HANDLE));
    thread_ids = (int*)malloc(number_of_threads * sizeof(int));
    square_counter = (int*)calloc(number_of_threads, sizeof(int));
    args = malloc(number_of_threads * sizeof(int[2]));

    if (h_child_thread == NULL || thread_ids == NULL || square_counter == NULL)
    {
        printf(RED "Memory Allocation failed\n" WHITE);
        return 1;
    }

    /* printf("Hello from the parent thread!\n"); */
    printf(CYAN "Parent thread creating child threads\n" WHITE);

    for (i = 0; i < number_of_threads; i++)
    {
        thread_ids[i] = i + 1;

        args[i][0] = thread_ids[i];
        args[i][1] = size;
        
        h_child_thread[i] = CreateThread(
            NULL,
            0,
            ChildThreadFunction,
            args[i],
            0,
            NULL);
            
        if (h_child_thread[i] == NULL)
        {
            printf(RED "Failed to create child thread\n" WHITE);
            return 1;
        }
        else
        {
            printf("      Thread %d: Created successfully\n",
                thread_ids[i]);
        }
    }

    printf(GREEN "All child threads created\n" WHITE);        
    printf(RED "Parent sleeping until the deadline of %d\n" WHITE,
        deadline);
    Sleep(deadline * 1000);
    keep_running = 0;

    printf(GREEN "All child threads finished\n");
    printf(RED "Deadline of %d seconds reached\n" WHITE,
        deadline);
    /* Wait for child threads to complete */
    Sleep(500);

    for (i = 0; i < number_of_threads; i++)
    {
        CloseHandle(h_child_thread[i]);
        printf("      Thread %d: Closed successfully\n",
            thread_ids[i]);
    }

    printf(GREEN "All child threads closed\n");
    printf(CYAN "Parent summary of square calls:\n" WHITE);
    for (i = 0; i < number_of_threads; i++) {
        printf("   Thread %d: %d square calls\n", i + 1, square_counter[i]);
    }

    free(h_child_thread);
    free(thread_ids);
    free((void*)square_counter);
    free(args);

    /* parent end time */
    GetSystemTime(&parent_end_time);
    SystemTimeToFileTime(&parent_end_time, &parent_end_ft);
    parent_end_uli.LowPart = parent_end_ft.dwLowDateTime;
    parent_end_uli.HighPart = parent_end_ft.dwHighDateTime;

    /* parent elapsed time */
    parent_elapsed_ms = (double)(parent_end_uli.QuadPart -
        parent_start_uli.QuadPart) / 10000.0;

    printf(CYAN "Parent thread completed in %.2f ms (%.2f seconds)\n" WHITE, 
           parent_elapsed_ms, parent_elapsed_ms / 1000.0);
    return 0;
}

int main(int argc, char *argv[])
{
    HANDLE h_parent_thread;
    DWORD parent_thread_id;

    int number_of_threads;
    int deadline;
    int size;

    int input_args[3]; /* To send as an array of inputs */

    SYSTEMTIME program_start_time, program_end_time;
    FILETIME program_start_ft, program_end_ft;
    ULARGE_INTEGER program_start_uli, program_end_uli;
    double total_elapsed_ms;

    if (argc != 4)
    {
        printf(RED "Error: Expecting 3 arguments\n" WHITE);
        return 1;
    }
    else
    {
        number_of_threads = strtol(argv[1], NULL, 0);
        deadline = strtol(argv[2], NULL, 0);
        size = strtol(argv[3], NULL, 0);

        if (number_of_threads <= 0 || number_of_threads > 1000)
        {
            printf(RED "Error: Number of threads must be between 1 and 1000\n" WHITE);
            return 1;
        }

        if (deadline <= 0 || deadline > 120)
        {
            printf(RED "Error: Deadline must be between 1 and 120 seconds\n" WHITE);
            return 1;
        }

        if (size <= 0 || size > 10000)
        {
            printf(RED "Error: Size must be between 1 and 10000\n" WHITE);
            return 1;
        }

        printf(CYAN "Inputs received...\n");
        printf("  Number of Threads: %d\n", number_of_threads);
        printf("  Deadline (seconds): %d\n", deadline);
        printf("  Size for Square(): %d\n", size);
        printf(WHITE "\n");
    }

    /* program start time */
    GetSystemTime(&program_start_time);
    SystemTimeToFileTime(&program_start_time, &program_start_ft);
    program_start_uli.LowPart = program_start_ft.dwLowDateTime;
    program_start_uli.HighPart = program_start_ft.dwHighDateTime;


    input_args[0] = number_of_threads;
    input_args[1] = deadline;
    input_args[2] = size;

    h_parent_thread = CreateThread(
        NULL,                   /* default security attributes */
        0,                      /* use default stack size */
        ParentThreadFunction,	/* thread function name */
        input_args,             /* argument to thread function */
        0,                      /* use default creation flags */
        &parent_thread_id);     /* returns the thread identifier */

    if (h_parent_thread == NULL)
    {
        printf(RED "Failed to create parent thread\n" WHITE);
        return 1;
    }
    else
    {
        /* printf("Thread %lu: Parent created successfully\n",
            parent_thread_id); */
        printf(GREEN "Parent Thread created\n" WHITE);
    }

    Sleep((deadline + 1) * 1000); /* Wait for parent thread to complete */
    CloseHandle(h_parent_thread);
    /* printf("Thread %lu: Parent closed successfully\n",
        parent_thread_id); */
    printf(GREEN "Parent Thread closed\n" WHITE);

    /* program end time */
    GetSystemTime(&program_end_time);
    SystemTimeToFileTime(&program_end_time, &program_end_ft);
    program_end_uli.LowPart = program_end_ft.dwLowDateTime;
    program_end_uli.HighPart = program_end_ft.dwHighDateTime;

    /* elapsed time */
    total_elapsed_ms = (double)(program_end_uli.QuadPart -
        program_start_uli.QuadPart) / 10000.0;
    printf(YELLOW "\n=== PROGRAM SUMMARY ===\n");
    printf("   Total program elapsed time: %.2f ms (%.2f seconds)\n\n" WHITE, 
           total_elapsed_ms, total_elapsed_ms / 1000.0);


    return 0;
}
