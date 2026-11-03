
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
#include "kernel/stat.h"
#include "user/user.h"
#include "user/mutex.h"


extern void thread_init(void);
extern void thread_create(void (*func)());
extern void thread_schedule(void);
extern void thread_yield(void);

int mtx;
int buffer_count = 0;
int max_space = 10;

void producer() {
    for(int i = 0; i < 20; i++) {
        mtx_lock(mtx);
        
        /* since the assignment said we don't need counting semaphores, just 
         integers updated atomically.And we hold the mutex lock here, 
         reading and writing buffer_count is safe from race conditions.*/
        if(buffer_count < max_space) {
            buffer_count++;
            printf("Producer: added item. Buffer count: %d\n", buffer_count);
        }
        
        mtx_unlock(mtx);
        
        // yield to multiplex the CPU and let the consumer run, 
        // used the implmentation guide from xv6 textbook chapt 7.
        thread_yield(); 
    }
}

void consumer() {
    for(int i = 0; i < 20; i++) {
        mtx_lock(mtx);
        
        // only consume if the buffer actually has something
        if(buffer_count > 0) {
            buffer_count--;
            printf("Consumer: took item. Buffer count: %d\n", buffer_count);
        }
        
        mtx_unlock(mtx);
        
        // yield CPU back to producer
        thread_yield(); 
    }
    exit(0); 
}

int main(int argc, char *argv[]) {
    // start unlocked (0)
    mtx = mtx_create(0); 
    
    thread_init();
    
    // create sibling threads sharing the same memory and mutex
    thread_create(producer);
    thread_create(consumer);
    
    // start the thread scheduler
    thread_schedule();
    
    exit(0);
}
