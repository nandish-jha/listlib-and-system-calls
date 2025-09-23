#ifndef PARTA_H
#define PARTA_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef BUILD_PARTA1
    #include <windows.h>
    #include <process.h>
#endif

#ifdef BUILD_PARTA2
    #include <standards.h>
    #include <unistd.h>
    #include <os.h>
#endif

#ifdef BUILD_PARTA3
    #include <pthread.h>
    #include <unistd.h>
#endif

#ifdef BUILD_PARTA4
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <unistd.h>
#endif

/* Global variables */
extern volatile int *square_counter;
extern __thread int current_thread;

#endif