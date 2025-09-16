/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <partA.h>

volatile int keep_running = 1;
volatile int *square_counter = NULL;
__thread int current_thread = -1;

void *ChildThreadFunction(void *arg)
{
    int size = *(int*)arg;
    int result = 0;

    printf("      Child Thread started\n");

    if (keep_running)
    {
        result = Square(size);
        printf("      Child thread: Square(%d) = %d\n",
            size, result);
        /* printf("      Child Thread says Hello\n"); */
        sleep(1);
    }

    printf("      Child Thread completed\n");
    return NULL;
}

void *ParentThreadFunction(void *arg)
{
    pthread_t child_thread;
    int create_child_r;

    int *size_ptr = (int*)arg;

    printf("   Parent thread started\n");
    
    create_child_r = pthread_create(
        &child_thread,
        NULL,
        ChildThreadFunction,
        size_ptr);

    if (create_child_r)
    {
        printf("Failed to create child thread\n");
        return NULL;
    }
    else
    {
        printf("   Child thread created successfully\n");
    }

    pthread_detach(child_thread);
    sleep(3);
    keep_running = 0;
    printf("   Child thread closed\n");

    printf("   Parent thread completed\n");
    return NULL;    
}

int main()
{
    pthread_t parent_thread;
    int creat_parent_r;

    int size = 6; /* hard coded for now */

    /* create parent thread */
    creat_parent_r = pthread_create(
        &parent_thread,
        NULL,
        ParentThreadFunction,
        &size);

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

    pthread_detach(parent_thread);
    sleep(6);
    printf("Parent thread closed\n");

    return 0;
}
