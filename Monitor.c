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

/* Includes for RTT will be activated in Phase 2 */
/* #include <rtt/rtt.h> */
/* #include <rtt/rttsem.h> */

LIST *cv_lists[MAX_CV];

/*
 * Purpose: Initializes the monitor's internal structures, including the lists 
 *          used for condition variables and (in phase 2) the semaphores.
 * Parameters: None
 * Returns: void
 */
void MonInit(int numConds) {
    int i;

    if (numConds <= 0 || numConds > MAX_CV) {
	fprintf(stderr, "Error: Invalid number of conditions\n");
	exit(1);
	} 

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
void MonEnter(void) {
    printf("MonEnter called.\n");
}

/*
 * Purpose: Acts as the exit gate for the monitor. Releases mutual exclusion
 *          so another waiting thread can enter.
 * Parameters: None
 * Returns: void
 */
void MonLeave(void) {
    printf("MonLeave called.\n");
}

/*
 * Purpose: Blocks the calling thread on a specific condition variable and 
 *          temporarily releases the monitor lock.
 * Parameters: cv - an integer representing the condition variable 
 * to wait on (0 to MAX_CV-1)
 * Returns: 0 on success, -1 if the cv index is out of bounds.
 */
int MonWait(int cv) {
    if (cv < 0 || cv >= MAX_CV) {
        fprintf(stderr, "Error: Invalid CV index %d\n", cv);
        return -1;
    }
    printf("MonWait called on cv %d.\n", cv);
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
int MonSignal(int cv) {
    if (cv < 0 || cv >= MAX_CV) {
        fprintf(stderr, "Error: Invalid CV index %d\n", cv);
        return -1;
    }
    printf("MonSignal called on cv %d.\n", cv);
    return 0;
}
