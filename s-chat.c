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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <os.h>
#include <standards.h>
#include <list.h>

#define MIN_PORT 30001
#define MAX_PORT 40000
#define THREAD_STACK 16384

typedef struct {
	int socketFd;
	struct sockaddr_storage remoteAddr;
	socklen_t remoteAddrLen;
	} ChatConfig;
static ChatConfig chatConfig;


PID keyboardPid;
PID receivePid;
PID screenPid;
PID sendPid;
PID serverPid;

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


void mainp(int argc, char *argv[]) {
	char *end;
	long localPort;
	long remotePort;
	int sockfd;
	struct sockaddr_in localAddr;
	struct addrinfo hints;
	struct addrinfo *remoteInfo;
	char remotePortStr[16];
	int result;
	int socketFlags;
		
	if (argc != 4) {
		fprintf(stderr, 
		"Usage: %s <localPort> <remoteHost> <remotePort>\n",
 		argv[0]);
	return;
	}
	

	localPort = strtol(argv[1], &end, 10);
	
	if (*argv[1] == '\0' || *end != '\0' || localPort < MIN_PORT ||
	localPort > MAX_PORT) {
		fprintf(stderr, "Error: invalid local port\n");
		return;
	}


	if (argv[2][0] == '\0') {
		fprintf(stderr, "Error: Remote hostname cant be empty\n");
		return;
	}


	remotePort = strtol(argv[3], &end, 10);
	
	if (*argv[3] == '\0' || *end != '\0' ||
		remotePort < MIN_PORT || remotePort > MAX_PORT) {
		fprintf(stderr, "Error: invalid remote port\n");
		return;	
		}

	printf("Local port: %ld\n", localPort);
	printf("Remote host: %s\n", argv[2]);
	printf("Remote port: %ld\n", remotePort);


	/* Create a UDP socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sockfd == -1) {
		perror("socket");
		return;
	}
	
	printf("UDP socket created successefully\n");
	
	/* Configure the local UDP address */
	memset(&localAddr, 0, sizeof(localAddr));

	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons((unsigned short)localPort);
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	/* Bind the socket to the local part */
	if (bind(sockfd, (struct sockaddr *) &localAddr, 
		sizeof(localAddr)) == -1) {
		perror("bind");
		close(sockfd);
		return;
	}

	printf("UDP socket bound to port %ld\n", localPort);
	
	
	/* get the socket's current flags */
	socketFlags = fcntl(sockfd, F_GETFL, 0);
	
	if (socketFlags == -1) {
		perror("fcntl F_GETFL");	
		close(sockfd);
		return;
	}


	/*enable non blocking UDP input */
	if (fcntl(sockfd, F_SETFL, socketFlags | O_NONBLOCK) == -1) {
		perror("fcntl F_SETFL");
		close(sockfd);
		return;
	}


	/* Convert the remote port to a string for getaddrinfo() */
	snprintf(remotePortStr, sizeof(remotePortStr),"%ld", remotePort);

	/* Configure hostname resolution for IPv4 UDP */
	memset(&hints, 0, sizeof(hints));
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	
	/* resolve the remote hostname and port */
	result = getaddrinfo(argv[2], remotePortStr, &hints, &remoteInfo);

	if (result != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
		close(sockfd);
		return;
	}
	
	printf("Remote hostname resolved seccessfully\n");
	

 	/* Save networking information for the worker thread */
	chatConfig.socketFd = sockfd;
	chatConfig.remoteAddrLen = remoteInfo->ai_addrlen;

	memcpy(&chatConfig.remoteAddr, remoteInfo->ai_addr, 
		remoteInfo->ai_addrlen);


	/* Create the coordinating server first */
	serverPid = Create((void (*)()) ServerThread, THREAD_STACK, 
		"Server", NULL, NORM, SYS);

	if (serverPid == PNUL) {
		fprintf(stderr, "Error creating server thread\n");
		freeaddrinfo(remoteInfo);
		close(sockfd);
		return;
		}
	
	freeaddrinfo(remoteInfo);


	/* create four worker threads */                                        
        keyboardPid = Create((void (*)()) KeyboardThread, THREAD_STACK,         
                "Keyboard", NULL, NORM, USR);                                   
                                                                                
        receivePid = Create((void(*)()) ReceiveThread, THREAD_STACK,            
                "Receiver", &chatConfig, NORM, USR);                                   
        screenPid = Create((void(*)()) ScreenThread, THREAD_STACK,              
                "Screen", NULL, NORM, USR);                                     
                                                                                
        sendPid = Create((void(*)()) SendThread, THREAD_STACK,                  
                "Sender", &chatConfig, NORM, USR);                                     
                                                                                
        if (keyboardPid == PNUL || receivePid == PNUL || screenPid == PNUL ||   
                sendPid == PNUL) {                                              
                fprintf(stderr, "Error creating worker threads\n");             
                return;                                                         
        }                                                                       
                                                                                
        printf("all five UBC threads created successfully");  


	/* networking and thread setup */
	if (close(sockfd) == -1) {
		perror("close");
		return;
	}
	

	
	return;

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
 * rescfrom() checks whethe a UDP message has arrived
 * if a message arrives it stores the data in buffer
 * if no message EAGAIN or EWOULDBLOCK is expected
 * Sched() alllows another UBC thread to execute
 * other errors r reported using perror()  
 *
*/
void ReceiveThread(void *arg) {
	
    ChatConfig *config = (ChatConfig *)arg;


    char buffer[1024];
    struct sockaddr_storage senderAddr;
    socklen_t senderLen;
    ssize_t bytesReceived;

    for (;;) {
        senderLen = sizeof(senderAddr);
        bytesReceived = recvfrom( config->socketFd, buffer,
            sizeof(buffer) - 1, 0, (struct sockaddr *)&senderAddr,&senderLen);

        if (bytesReceived >= 0) {
            buffer[bytesReceived] = '\0';
            printf("Received: %s\n", buffer);

            /* TODO: Forward message to ServerThread using Send() */

        }

        else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("recvfrom");
            break;

        }

        /* Let other UBC threads execute */
        Sched();



    }
	
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
