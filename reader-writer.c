/*# Dwight Makaroff   */
/*# student number    */
/*# nsid              */
/*# November 7, 2008  */
/* modified to use RT-Threads October 2018 */
/* modified back for 2019 */
/* No part of this program may be used for any profit or example for any
   purpose other than for help in A2 for CMPT 332 at the University
   of Saskatchewan in the fall term of 2019-20 without the expressed
   written consent of the author.

   *********** reader-writer.c ***********
*/
/* CMPT 332 GROUP 18 Change, Fall 2026 */
/* Emilio Montelongo Luna                                                       
 * 11317059                                                                     
 * rjq760                                                                       
 * CMPT 332-Fall-2026                                                           
 */                                                                             
                                                                                
                                                                                
/*                                                                              
 Ahmet Guldadov                                                                 
 Student Number: 11369772                                                       
 NSID: six402                                                                   
 CMPT 332-Fall-2026                                                             
**/ 


#include <stdio.h>
#include <stdlib.h>
#include <rtthreads.h>
#include <standards.h>

#include <reader-writer-monitor.h>

#define SLEEPMAX 20

void reader(void *arg)
{
  long myId;
  
  myId = (long) arg;
  
  for(;;)
    {
      printf("%ld start read\n", myId);
      StartRead();
      printf("%ld Reading\n", myId);
      RttSleep((int) (rand() % SLEEPMAX));
      StopRead();
      printf("%ld stop read\n", myId);
      RttSleep((int) (rand() % SLEEPMAX));
    }
}

void writer(void *arg)
{
  long myId;
  myId = (long) arg;
  
  for(;;)
    {
      printf("%ld start write\n", myId);
      StartWrite();
      printf("%ld writing\n", myId);
      RttSleep((int) (rand() % SLEEPMAX*5));
      StopWrite();
      printf("%ld stop write\n", myId);
      RttSleep((int) (rand() % SLEEPMAX*6));
    }
  
}

int mainp()
{
    RttSchAttr attrs;
    RttThreadId tempPid, temp2, temp3; 
    
    Initialize();

    attrs.startingtime = RTTZEROTIME;
    attrs.priority = RTTNORM;
    attrs.deadline = RTTNODEADLINE;
    
    setbuf(stdout, 0);
    srand(71);

    /* changed thread creation using RttCreate */
    if (RttCreate(&tempPid, (void(*)()) reader, 16000, "R1", 
(void *) 1000, attrs, RTTUSR) == RTTFAILED) 
        perror("Create R1");
        
    if (RttCreate(&temp2, (void(*)()) writer, 16000, "W1", 
(void *) 500, attrs, RTTUSR) == RTTFAILED) 
        perror("Create W1");
        
    if (RttCreate(&temp3, (void(*)()) reader, 16000, "R2", 
(void *) 1001, attrs, RTTUSR) == RTTFAILED) 
        perror("Create R2");
        
    if (RttCreate(&temp3, (void(*)()) reader, 16000, "R3", 
(void *) 1002, attrs, RTTUSR) == RTTFAILED) 
        perror("Create R3");
        
    if (RttCreate(&temp3, (void(*)()) writer, 16000, "W2",
(void *) 501, attrs, RTTUSR) == RTTFAILED) 
        perror("Create W2");
        
    printf("Reader and Writer processes created\n");

    return(0);
}
