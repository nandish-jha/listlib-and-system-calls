/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <partA.h>

/* process creation (for doing it globally) */
pid_t child_process;

volatile int keep_running = 1;
volatile int *square_counter = NULL;
__thread int current_thread = -1;

void alarm_handler(int sig)
{
    printf("*** Deadline reached, killing child process ***\n");
    kill(child_process, SIGKILL);
}

int main()
{
    /* process creation (to be done before any code) */
    /* pid_t child_process; */

    int size = 6;
    int result = 0;

    printf("Parent/main process started\n");
    child_process = fork();

    if (child_process < 0)
    {
        perror("Process fork failed\n");
        return 1;
    }
    else if (child_process == 0)   /* <------ child process */
    {
        printf("      Child process started (PID: %d)\n",
            getpid());

        result = Square(size);
        printf("         Child process: Square(%d) = %d\n",
            size, result);

            /* to simulate some work that CANNOT finish before dealine */
            /* sleep(6); */
            /* to simulate some work that CAN finish before dealine */
            sleep(3);

        printf("      Child process completed (PID: %d)\n",
            getpid());
        return 0;
    }
    else                            /* <------ parent process */
    {
        printf("   Hello from the parent process (PID: %d)\n",
            getpid());
        printf("   Created a child process (PID: %d)\n",
            child_process);

        /* setting alarm for 5 seconds */
        signal(SIGALRM, alarm_handler);
        alarm(5);

        wait(NULL); /* while the parent waits, the child runs */
        printf("Parent process completed\n");
    }

    return 0;
}
