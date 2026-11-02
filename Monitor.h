
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

void RttMonInit(int numConds);
void RttMonEnter(void);
void RttMonLeave(void);
int RttMonWait(int cv);
int RttMonSignal(int cv);

#endif
