
/* CMPT 332 GROUP 18 Change, Fall 2026 */
/* Emilio Montelongo Luna
 * Student Number: 11317059
 * NSID:rjq760
 * CMPT 332-Fall-2026
 */


/*
 Ahmet Guldadov
 Student Number: 11369772
 NSID: six402
 CMPT 332-Fall-2026
**/

#include "kernel/types.h"
#include "user/user.h"
#include "user/mutex.h"

#define MAX_MTX 32

static uint locks[MAX_MTX];
static int active[MAX_MTX]; 

extern void thread_yield(void);

int mtx_create(int locked) {
    for(int i = 0; i < MAX_MTX; i++) {
        if(__sync_bool_compare_and_swap(&active[i], 0, 1)) {
            locks[i] = locked ? 1 : 0;
            return i; 
        }
    }
    return -1; 
}

int mtx_lock(int lock_id) {
    if(lock_id < 0 || lock_id >= MAX_MTX || !active[lock_id]) return -1;
    
    /* The xv6 textbook (Chapter 6) notes that normal C assignment isn't atomic 
     on multiprocessors. We use the __sync builtin which compiles to 
     the RISC-V amoswap instruction to swap and check in one step.
     If it's locked, we yield instead of spinning forever 
     to let other threads run.*/
    while(__sync_lock_test_and_set(&locks[lock_id], 1) != 0) {
        thread_yield();
    }
    
    // it aalso says we need a memory barrier to prevent the compiler
    // from re-ordering load/stores across the lock.
    __sync_synchronize();
    return 0;
}

int mtx_unlock(int lock_id) {
    if(lock_id < 0 || lock_id >= MAX_MTX || !active[lock_id]) return -1;
    
    // Memory barrier again before releasing
    __sync_synchronize();
    
    __sync_lock_release(&locks[lock_id]);
    return 0;
}
