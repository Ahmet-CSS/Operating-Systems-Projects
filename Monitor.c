/*
 * Emilio Montelongo Luna
 * 11317059
 * rjq760
 * CMPT 332 Fall-2026
 */
/*
 Ahmet Guldadov
 Student Number: 11369772
 NSID: six402
 CMPT 332-Fall-2026
**/
#include <stdio.h>
#include <stdlib.h>
#include "Monitor.h"
#include "list.h"
#include <rtthreads.h>

LIST *cv_lists[MAX_CV];


/* 
 * global mutex semaphor for mutual exclusion 
 * acrosl monitor routines.
 */
RttSem monitor_mutex;


/*
 * Purpose: Initializes the monitor's internal structures, including the lists 
 *          used for condition variables and (in phase 2) the semaphores.
 * Parameters: None
 * Returns: void
 */
void RttMonInit(int numConds) {
    int i;

    if (numConds <= 0 || numConds > MAX_CV) {
	fprintf(stderr, "Error: Invalid number of conditions\n");
	exit(1);
	} 

	/* allocateding the semaphore with inital val of 1, used the rttapi
 * document to use RTTPRIORITY rather than RTT_IPC_prio as 
 * the mode parameter */
	RttAllocSem(&monitor_mutex, 1, RTTPRIORITY);

    for (i = 0; i < numConds; i++) {
        cv_lists[i] = ListCreate();
        if (cv_lists[i] == NULL) {
            fprintf(stderr, "Error: Failed to create list for CV %d\n", i);
            exit(1);
        }
    }
    printf("MonInit called. Lists created.\n");
}

/*
 * Purpose: Acts as the entry gate for the monitor. Enforces mutual exclusion
 *          so only one thread can execute monitor code at a time.
 * Parameters: None
 * Returns: void
 */
void RttMonEnter(void) {
    printf("MonEnter called.\n");
    /* Wait on the monitor's global mutex to acquire exclusive access */
    RttP(monitor_mutex);
}


/*
 * Purpose: Acts as the exit gate for the monitor. Releases mutual exclusion
 *          so another waiting thread can enter.
 * Parameters: None
 * Returns: void
 */
void RttMonLeave(void) {
    printf("MonLeave called.\n");
    /* Signal the monitor's global mutex to let the next thread in */
    RttV(monitor_mutex);
}

/*
 * Purpose: Blocks the calling thread on a specific condition variable and 
 *          temporarily releases the monitor lock.
 * Parameters: cv - an integer representing the condition variable 
 * to wait on (0 to MAX_CV-1)
 * Returns: 0 on success, -1 if the cv index is out of bounds.
 */
int RttMonWait(int cv) {
    RttSem *private_sem;
    if (cv < 0 || cv >= MAX_CV) {
        fprintf(stderr, "Error: Invalid CV index %d\n", cv);
        return -1;
    }
    printf("MonWait called on cv %d.\n", cv);

    /* 
     * 
     * Create a specific semaphore for this thread so it can block itself. 
     * It starts locked (value 0). We then queue it from list library.
     */
    private_sem = (RttSem *)malloc(sizeof(RttSem));
    RttAllocSem(private_sem, 0, RTTPRIORITY);
    
    ListAppend(cv_lists[cv], (void *)private_sem);

    /* Release the monitor lock so other threads aren't deadlocked 
 	* while we sleep */
    RttMonLeave();

    /* 
     * sleep until another thread calls RttMonSignal and wakes 
     * this specific semaphore 
     */
    RttP(*private_sem);

    /* 
     *  
     * woke up the sem. Clean up our private 
     * semaphore to avoid memory leaks. 
     */
    RttFreeSem(*private_sem);
    free(private_sem);

    /* reacquire the monitor lock before executing the 
 	* rest of the critical section */
    RttMonEnter();
    return 0;
}

/*
 * Purpose: Wakes up a single thread waiting on the specified 
 * condition variable. 
 *          If no threads are waiting, it does nothing.
 * Parameters: cv - an integer representing the condition variable to 
 * signal (0 to MAX_CV-1)
 * Returns: 0 on success, -1 if the cv index is out of bounds.
 */
int RttMonSignal(int cv) {
    RttSem *popped_sem;
    if (cv < 0 || cv >= MAX_CV) {
        fprintf(stderr, "Error: Invalid CV index %d\n", cv);
        return -1;
    }
    printf("MonSignal called on cv %d.\n", cv);

    /* 
     * Check ifthe list library actually has threads waiting on this CV. 
     * If so, pull the first one out and wake it up using RttV.
     */
    if (ListCount(cv_lists[cv]) > 0) {
        ListFirst(cv_lists[cv]);
        popped_sem = (RttSem *)ListRemove(cv_lists[cv]);
        
        RttV(*popped_sem);
    }
    return 0;
}
