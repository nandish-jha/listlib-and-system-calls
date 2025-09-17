/*
NANDISH JHA NAJ474 11282001
*/

/* partA1.c
 * Multi-threaded driver: parent spawns N child threads, each calls
 * Square(1..size) repeatedly until the parent signals a deadline.
 * Usage: partA1 <number_of_threads> <deadline_seconds> <size>
 */

#include <partA.h>
#include <square.h>

/* Global run flag: parent flips to 0 to request children stop. */
volatile int keep_running = 1;

/* Per-thread call counters (allocated by parent). Square() updates
 * the appropriate index using the thread-local `current_thread`. */
volatile int *square_counter = NULL;

/* Thread-local index for the running child thread (0-based). This
 * lets Square() know which counter slot to increment safely per-thread. */
__thread int current_thread = -1;

/* Child thread: run Square(i) for i=1..size until the parent signals
 * `keep_running` == 0, then report elapsed time and call counts. */
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

    /* Publish this thread's index so Square() can update the right
     * counter. Brief Sleep yields to ensure TLS is visible to other
     * operations that might read it (small, deliberate pause). */
    current_thread = thread_index;
    Sleep(1);

    for (i = 1; i <= incoming_size; i++)
    {
        /* If parent still allows running, perform the work. Otherwise
         * stop early and report where we stopped. */
        if (keep_running)
        {
            Square(i);
        }
        else
        {
            printf("   Thread %d stopped at Square(%d)\n",
                incoming_thread_id, i);
            break;
        }
    }

    /* end times */
    GetSystemTime(&end_time);
    SystemTimeToFileTime(&end_time, &end_ft);
    end_uli.LowPart = end_ft.dwLowDateTime;
    end_uli.HighPart = end_ft.dwHighDateTime;

    /* Compute elapsed time in milliseconds (FILETIME units -> 100 ns). */
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
    /* Each child takes a pair: (thread_id, size). Use a contiguous
     * array of pairs so lifetime is simple and pointer passing remains
     * stable during thread creation. */
    int (*args)[2];
    int i;

    SYSTEMTIME parent_start_time, parent_end_time;
    FILETIME parent_start_ft, parent_end_ft;
    ULARGE_INTEGER parent_start_uli, parent_end_uli;
    double parent_elapsed_ms;
    
    /* parent start time (used only for reporting total parent elapsed) */
    GetSystemTime(&parent_start_time);
    SystemTimeToFileTime(&parent_start_time, &parent_start_ft);
    parent_start_uli.LowPart = parent_start_ft.dwLowDateTime;
    parent_start_uli.HighPart = parent_start_ft.dwHighDateTime;

    /* Allocate resources: handles, ids, per-thread counters, and args. */
    h_child_thread = (HANDLE*)malloc(number_of_threads * sizeof(HANDLE));
    thread_ids = (int*)malloc(number_of_threads * sizeof(int));
    square_counter = (int*)calloc(number_of_threads, sizeof(int));
    args = malloc(number_of_threads * sizeof(int[2]));

    /* Basic allocation failure check; keep reporting concise and actionable. */
    if (h_child_thread == NULL || thread_ids == NULL || square_counter == NULL)
    {
        printf("Memory Allocation failed\n");
        return 1;
    }

    /* printf("Hello from the parent thread!\n"); */
    printf("Parent thread creating child threads\n");

    for (i = 0; i < number_of_threads; i++)
    {
        thread_ids[i] = i + 1;

        args[i][0] = thread_ids[i];
        args[i][1] = size;

        /* Pass pointer to args[i] (its storage is in the `args` array
         * which persists until after we join/close the child threads). */
        h_child_thread[i] = CreateThread(
            NULL,
            0,
            ChildThreadFunction,
            args[i],
            0,
            NULL);
            
        if (h_child_thread[i] == NULL)
        {
            printf("Failed to create child thread\n");
            return 1;
        }
        else
        {
            printf("      Thread %d: Created successfully\n",
                thread_ids[i]);
        }
    }

    printf("All child threads created\n");        
    /* Sleep until deadline then request children to stop by flipping the
     * shared `keep_running` flag. */
    printf("Parent sleeping until the deadline of %d\n",
        deadline);
    Sleep(deadline * 1000);
    keep_running = 0;

    printf("All child threads finished\n");
    printf("Deadline of %d seconds reached\n",
        deadline);
    /* Wait a short grace period for child threads to observe flag and
     * finish; a proper join would be better, but CloseHandle here is
     * sufficient for this assignment's intent. */
    Sleep(500);

    for (i = 0; i < number_of_threads; i++)
    {
        CloseHandle(h_child_thread[i]);
        printf("      Thread %d: Closed successfully\n",
            thread_ids[i]);
    }

    printf("All child threads closed\n");
    printf("Parent summary of square calls:\n");
    for (i = 0; i < number_of_threads; i++) {
        printf("   Thread %d: %d square calls\n", i + 1, square_counter[i]);
    }

    /* Clean up heap allocations and report timings. */
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

    printf("Parent thread completed in %.2f ms (%.2f seconds)\n", 
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

    /* Pack inputs for the parent thread. Note: `input_args` must remain
     * valid while the parent thread runs — main waits before exiting. */
    int input_args[3]; /* To send as an array of inputs */

    SYSTEMTIME program_start_time, program_end_time;
    FILETIME program_start_ft, program_end_ft;
    ULARGE_INTEGER program_start_uli, program_end_uli;
    double total_elapsed_ms;

    /* Validate command-line arguments with clear, narrow ranges. */
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

    /* program start time */
    GetSystemTime(&program_start_time);
    SystemTimeToFileTime(&program_start_time, &program_start_ft);
    program_start_uli.LowPart = program_start_ft.dwLowDateTime;
    program_start_uli.HighPart = program_start_ft.dwHighDateTime;


    input_args[0] = number_of_threads;
    input_args[1] = deadline;
    input_args[2] = size;

    /* Create the parent thread and pass the input_args array. */
    h_parent_thread = CreateThread(
        NULL,                   /* default security attributes */
        0,                      /* use default stack size */
        ParentThreadFunction,	/* thread function name */
        input_args,             /* argument to thread function */
        0,                      /* use default creation flags */
        &parent_thread_id);     /* returns the thread identifier */

    if (h_parent_thread == NULL)
    {
        printf("Failed to create parent thread\n");
        return 1;
    }
    else
    {
        /* printf("Thread %lu: Parent created successfully\n",
            parent_thread_id); */
        printf("Parent Thread created\n");
    }

    Sleep((deadline + 1) * 1000); /* Wait for parent thread to complete */
    CloseHandle(h_parent_thread);
    /* printf("Thread %lu: Parent closed successfully\n",
        parent_thread_id); */
    printf("Parent Thread closed\n");

    /* program end time */
    GetSystemTime(&program_end_time);
    SystemTimeToFileTime(&program_end_time, &program_end_ft);
    program_end_uli.LowPart = program_end_ft.dwLowDateTime;
    program_end_uli.HighPart = program_end_ft.dwHighDateTime;

    /* elapsed time */
    total_elapsed_ms = (double)(program_end_uli.QuadPart -
        program_start_uli.QuadPart) / 10000.0;
    printf("\n=== PROGRAM SUMMARY ===\n");
    printf("   Total program elapsed time: %.2f ms (%.2f seconds)\n\n", 
           total_elapsed_ms, total_elapsed_ms / 1000.0);


    return 0;
}
