
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

#ifndef MONITOR_H
#define MONITOR_H

#define MAX_CV 10

void MonInit(int numConds);
void MonEnter(void);
void MonLeave(void);
int MonWait(int cv);
int MonSignal(int cv);

#endif
