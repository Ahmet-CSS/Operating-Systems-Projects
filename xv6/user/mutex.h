/* CMPT 332 GROUP 18 change, Fall 2026 */

#ifndef MUTEX_H
#define MUTEX_H



/* Emilio Montelongo Luna
 * Student Number: 11317059
 * NSID: rjq760
 * CMPT 332-Fall-2026
 */


/*
 Ahmet Guldadov
 Student Number: 11369772
 NSID: six402
 CMPT 332-Fall-2026
**/

int mtx_create(int locked);
int mtx_lock(int lock_id);
int mtx_unlock(int lock_id);

#endif
