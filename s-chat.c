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

 *  Assignment 2 Part C
 * Group 18 - Fall 2026
 *
 * UDP chat application using UBC pthreads
 *
 * Phase 1: Program skeleton and parameter validation
 */

#include <stdio.h>
#include <stdlib.h>


#define MIN_PORT 30001
#define MAX_PORT 40000

/**
 * Thread functions
 * implementation will be completed in final phase
 */


void KeyboardThread(void *arg);
void ReceiveThread(void *arg);
void ScreenThread(void *arg);
void SendThread(void *arg);
void ServerThread(void *arg);



/**
 * validate chat parameters and prepare the application
 * parameters: argc - argument count
 * 	       argv - commmand line arguments
 * Returns: 0 on success 1 on invalid input
 */


int main(int argc, char *argv[]) {
	char *end;
	long localPort;
	long remotePort;


	if (argc != 4) {
		fprintf(stderr, 
		"Usage: %s <localPort> <remoteHost> <remotePort>\n",
 		argv[0]);
	return 1;
	}
	

	localPort = strtol(argv[1], &end, 10);
	
	if (*argv[1] == '\0' || *end != '\0' || localPort < MIN_PORT ||
	localPort > MAX_PORT) {
		fprintf(stderr, "Error: invalid local port\n");
		return 1;
	}


	if (argv[2][0] == '\0') {
		fprintf(stderr, "Error: Remote hostname cant be empty\n");
		return 1;
	}


	remotePort = strtol(argv[3], &end, 10);
	
	if (*argv[3] == '\0' || *end != '\0' ||
		remotePort < MIN_PORT || remotePort > MAX_PORT) {
		fprintf(stderr, "Error: invalid remote port\n");
		return 1;	
		}

	printf("Local port: %ld\n", localPort);
	printf("Remote host: %s\n", argv[2]);
	printf("Remote port: %ld\n", remotePort);


	/* Phase 2: initialize networking and create five threads */

	return 0;

}




/*
 * Reads keybord input from user
 *
 */
void KeyboardThread(void *arg) {
	(void)arg;
	/*TODO read keyboard input */


}




/*
 * Receives incoming UDP messages.
 */
void ReceiveThread(void *arg) {
	(void)arg;
    /* TODO  Receive UDP packets. */
}




/*
 * Displays received messages.
 */
void ScreenThread(void *arg) {
	(void)arg;
    /* TODO Display messages on screen. */
}





/*
 * Sends outgoing UDP messages.
 */
void SendThread(void *arg) {
	(void)arg;
    /* TODO Send UDP packets. */
}



/*
 * Coordinates the other four threads.
 */
void ServerThread(void *arg) {
	(void)arg;
    /* TODO  Manage message queues and synchronization. */
}
