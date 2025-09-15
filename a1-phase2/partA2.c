/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <partA.h>

volatile int keep_running = 1;
volatile int *square_counter = NULL;
__thread int current_thread = -1;

PROCESS ChildThreadFunction(void *arg)
{
    int size = *(int *)arg;
    int result = 0;
    
    printf("      Child thread started\n");

    if (keep_running)
    {
        result = Square(size);
        printf("      Child thread: Square(%d) = %d\n", size, result);
        /* printf("      Child Thread says Hello\n"); */
        Sleep(1);
    }

    printf("      Child Thread completed\n");
    exit(0);
}

PROCESS ParentThreadFunction(void *arg)
{
    PID child_thread_r;

    int *size_ptr = (int *)arg;
    
    /* create parent thread */
    child_thread_r = Create(
        ChildThreadFunction,
        16384,
        "ChildThread",
        size_ptr,
        NORM,
        USR);
    
    if (child_thread_r == PNUL)
    {
        printf("Failed to create child thread\n");
        exit(1);
    }
    else
    {
        printf("   Child thread created successfully (PID: %ld)\n",
            child_thread_r);
    }

    Sleep(3);
    keep_running = 0;

    Kill(child_thread_r);
    exit(0);
}

int mainp()
{
    PID parent_thread_r;

    int size = 6;
    
    /* create parent thread */
    parent_thread_r = Create(
        ParentThreadFunction,
        16384,
        "ParentThread",
        &size,
        NORM,
        USR);
    
    if (parent_thread_r == PNUL)
    {
        printf("Failed to create parent thread\n");
        return 1;
    }
    else
    {
        printf("Parent thread created successfully (PID: %ld)\n",
            parent_thread_r);
    }

    usleep(600000); /* same as Sleep(6) - 6 ticks */
    return 0;
}
