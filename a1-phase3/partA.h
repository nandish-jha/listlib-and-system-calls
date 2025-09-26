/*
Nandish Jha NAJ474 11282001
Aakash Jana AAJ284 11297343
*/

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
    #include <sys/time.h>
    #include <unistd.h>
    #include <os.h>
#endif

#ifdef BUILD_PARTA3
    #include <sys/time.h>
    #include <pthread.h>
    #include <unistd.h>
#endif

#ifdef BUILD_PARTA4
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/time.h>
    #include <sys/shm.h>
    #include <sys/ipc.h>
    #include <signal.h>
    #include <unistd.h>
#endif

/* Global variables */
extern volatile int *square_counter;
extern __thread int current_thread;

#endif