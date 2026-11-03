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
#include <stdint.h>
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
#define REQUEST_OUTGOING 1 /* SendThread request a message to transmit */
#define REQUEST_INCOMING 2 /* ScreenThread request a message to display */
#define ADD_OUTGOING 3 /* KeyboardThread adds a message */
#define ADD_INCOMING 4 /* RecieveThread adds a received message */
#define REQUEST_EXIT 5 /* a thread requests termination */
#define MAX_MESSAGE 1024
#define REQUEST_EXIT 5 /* thread request termination */
#define REQUEST_WORKER_DONE 6 /* worker has finished cleanup */


typedef struct {
	char text[MAX_MESSAGE];
	unsigned int seconds;
	unsigned int microseconds;
} ChatMessage;


typedef struct {
	int type;
	ChatMessage message;
} ChatRequest;




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
static int chatShuttingDown = 0;

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

		/*close(sockfd);*/
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



	return;

}




/*
 * Reads keybord input from user
 *
 */
void KeyboardThread(void *arg) {
	ChatRequest request;
	struct timeval timestamp;
	char input[MAX_MESSAGE];
	int inputLength;
	int length;
	int flags;
	int result;
  	(void)arg;

	/* save the original keyboard settings */
	flags = fcntl(STDIN_FILENO, F_GETFL, 0);

	if(flags == -1) {
		perror("KeyboardThread: fcntl");
		return;
	}
	/* make keybaord input nonbloccking */
	if(fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) {
		perror("KeyboardThread: O_NONBLOCK");
		return;
	}

	/*Read keyboard input and build complete lines */
	inputLength = 0;
	for(;;) {
		char ch;
		ssize_t bytesRead;

		bytesRead = read(STDIN_FILENO, &ch, 1);
 		if (bytesRead == 1) {

		/* a complete message has been entered */
		if (ch == '\n') {
			input[inputLength] = '\0';
			if(strcmp(input, "/quit")  == 0){
				printf("Chat shutdown requested\n");
				
				memset(&request, 0, sizeof(request));
				request.type = REQUEST_EXIT;
				length = sizeof(request);
				Send(serverPid, &request, &length);
				break;
			} 

			

			/* record the time when the message was entered */
			if(gettimeofday(&timestamp, NULL) == -1) {
				perror("KeyboardThread: gettimeofday");
				break;
			}



		/* Prepare the request for ServerThread */
		memset(&request, 0, sizeof(request));
		request.type = ADD_OUTGOING;
		strncpy(request.message.text, input, MAX_MESSAGE - 1);

		request.message.seconds = (unsigned int)timestamp.tv_sec;
		request.message.microseconds = (unsigned int)timestamp.tv_usec;

		/* send the completed message to the server */
		length = sizeof(request);
		Send(serverPid, &request, &length);
		/* prepare for next message */
		inputLength = 0;
		} else if (inputLength < MAX_MESSAGE - 1) {

			input[inputLength] = ch;
			inputLength++;
		}
		} else if (bytesRead == 0) {
		/* end of input */
			break;
		} else if (errno == EAGAIN || errno == EWOULDBLOCK ||
		errno == EINTR ) {
		/* no input available, let other UBC threads run */
		Sched();
		} else {
		perror("KeyboardThread: read");
		break;
		}

	}
	
	result = fcntl(STDIN_FILENO, F_SETFL, flags);

	if(result == -1) {
	perror("KeyboardThread: restore flags");
	}
	
	/* request shutdown when keyboard input reaches EOF */
	if (!chatShuttingDown) {
		memset(&request, 0, sizeof(request));
		request.type = REQUEST_EXIT;
		length = sizeof(request);
		Send(serverPid, &request, &length);
	}


	
	/* Notify ServerThread that keyboard cleanup is complete */
	memset(&request, 0, sizeof(request));
	request.type = REQUEST_WORKER_DONE;
	length = sizeof(request);
	Send(serverPid, &request, &length);



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
    ChatRequest request;
    unsigned char buffer[12 + MAX_MESSAGE];
    uint64_t seconds;
    uint32_t microseconds;
    ssize_t bytesReceived;
    size_t textLength;
    int length;
    int i;
    struct sockaddr_storage senderAddr;
    socklen_t senderLen;



    for (;;) {
       

	if (chatShuttingDown) {
		break;
	}
	senderLen = sizeof(senderAddr); 
        bytesReceived = recvfrom( config->socketFd, buffer,

        sizeof(buffer), 0, (struct sockaddr *)&senderAddr,&senderLen);


        if (bytesReceived < 0) {
		if (errno != EAGAIN ||  errno == EWOULDBLOCK ||
			errno == EINTR) {
		Sched();
		continue;
		}
		perror("ReceiveThread: recvfrom");
		break;
		}
	if (bytesReceived < 12 ) {
		fprintf(stderr, "ReceiveThread: invlaid packet\n");
		continue;
		}
	/* decode the 8 byte timestamp in network byte order */
		seconds = 0;
		for (i = 0; i < 8; i++) {
        	seconds = (seconds << 8) | (unsigned char)buffer[i];

    }
    /* Decode the 4 byte microseconds field */
    microseconds = 0;
    for (i = 8; i < 12; i++) {
        microseconds = (microseconds << 8) |(unsigned char)buffer[i];
    }

    if (microseconds >= 1000000) {
        fprintf(stderr, "ReceiveThread: invalid timestamp\n");
        continue;

    }

    /* Extract the message text*/
    textLength = (size_t)bytesReceived - 12;
    if (textLength >= MAX_MESSAGE) {
        fprintf(stderr, "ReceiveThread: message too long\n");

        continue;
    }

    memset(&request, 0, sizeof(request));
    request.type = ADD_INCOMING;
    memcpy(request.message.text, buffer + 12, textLength);
    request.message.text[textLength] = '\0';
    request.message.seconds = (unsigned int)seconds;
    request.message.microseconds = microseconds;
    /* forward the message to ServerThread using UBC IPC */
    length = sizeof(request);
    Send(serverPid, &request, &length);
    /* allow other UBC threads to execute */
    Sched();

    }
	/* notify server that receiver has stopped */
	memset(&request, 0, sizeof(request));
	request.type = REQUEST_WORKER_DONE;
	length = sizeof(request);
	Send(serverPid, &request, &length);

}




/*
 * Displays received messages.
 */
void ScreenThread(void *arg) {
	ChatRequest request;
	ChatMessage *message;
	int length;
	(void)arg;


	for(;;) {
		
		if (chatShuttingDown)	{
			break;
			}


		/*request the oldest incoming message */

		memset(&request, 0, sizeof(request));
		request.type = REQUEST_INCOMING;

		length = sizeof(request);

		message = (ChatMessage*)Send(serverPid, &request, &length);

		if (message == NULL) {
			fprintf(stderr, "ScreenThread: no server reply\n");
			break;
		}

		/* Display the sender's timestamp and message */
		printf("[%u.%06u] %s\n", message->seconds,
			message->microseconds, message->text);
		fflush(stdout);

		/* server transferred ownership of this message */
		free(message);
	}
	/* notify ServerThread that screen thread has stopped */
	memset(&request, 0, sizeof(request));
	request.type = REQUEST_WORKER_DONE;
	length = sizeof(request);
	
	Send(serverPid, &request, &length);

}





/*
 * Sends outgoing UDP messages
 * sets the request type to REQUEST_OUTGOING
 * it calls Send(serverPid, ....)
 * The sending thread blocks until ServerThread() replies
 * The reply will contain the next outgoing message
 *
 */
void SendThread(void *arg) {

	ChatConfig *config = (ChatConfig *)arg;
	ChatRequest request;
	ChatMessage *message;
	int length;
	unsigned char packet[12 + MAX_MESSAGE];
	uint64_t seconds;
	unsigned int microseconds;
	size_t textLength;
	size_t packetLength;
	ssize_t bytesSent;
	int i;

	(void)config;


	for(;;) {

	if (chatShuttingDown) {	
		break;
	}

	/* Request the next outgoing message */
	memset(&request, 0, sizeof(request));
	request.type = REQUEST_OUTGOING;
	length = sizeof(request);
	message = (ChatMessage *)Send(serverPid, &request, &length);
	if (message == NULL) {
            fprintf(stderr, "SendThread: no server reply\n");
            break;
        }
        /*
         * Encode the message and timestamp
         * Transmit the packet using sendto()
         */
	seconds = (uint64_t)message->seconds;

	for (i = 7; i >= 0; i--) {
		packet[i] = (unsigned char)(seconds & 0xFF);
    		seconds  = seconds >> 8;
	}
	/* encode microseconds as 4 bytes in network byte order */
	microseconds = message->microseconds;
	for (i = 11; i >= 8; i--) {

		packet[i] = (unsigned char)(microseconds & 0xFF);
		microseconds  = microseconds >> 8;

	}
	/* copy the message text after the timestamp */
	textLength = strlen(message->text);
	memcpy(packet + 12, message->text, textLength);
	packetLength = 12 + textLength;

	/* send the encoded message to the remote computer */
	for (;;) {

		bytesSent = sendto(config->socketFd, packet, packetLength, 0,
			(struct sockaddr*)&config->remoteAddr,
			config->remoteAddrLen);

	if (bytesSent >= 0) {
		break;
	}

	if (errno == EAGAIN || errno == EWOULDBLOCK ) {
		/* non blocking socket retry later */
		Sched();
		continue;
		}
		perror("SendThread: sendto");
		break;

	}
	free(message);
    }
	/* Notify ServerThread that sender has stopped */
	memset(&request, 0, sizeof(request));
	request.type = REQUEST_WORKER_DONE;
	length = sizeof(request);
	Send(serverPid, &request, &length);
}




void ServerThread(void *arg) {

    /*  Manage message queues and synchronization */
	LIST *incomingQueue;
	LIST *outgoingQueue;
	ChatRequest request;
	ChatRequest *receivedRequest;
	ChatMessage *newMessage;
	ChatMessage *queuedMessage;
	PID senderPid;
	PID waitingSender;
	PID waitingScreen;
	int length;
	int replyStatus;
	int delivered;
	int workersFinished;
	(void)arg;
	waitingSender = PNUL;
	waitingScreen = PNUL;
	workersFinished = 0;


	/* Create the incoming and outgoing message queues */
	incomingQueue = ListCreate();
	outgoingQueue = ListCreate();

	if (incomingQueue == NULL || outgoingQueue == NULL) {
		fprintf(stderr, "ServerThread: ListCreate failed\n");
		return;
	}

	printf("Server: message queues initialized \n");


	for (;;) {
		/*length = sizeof(request);
		Receive(&senderPid, &length); */
	length = 0;

	receivedRequest = (ChatRequest *)Receive(&senderPid, &length);

	if (receivedRequest == NULL || length != sizeof(ChatRequest)) {
		fprintf(stderr, "ServerThread: invalid request\n");
		Reply(senderPid, NULL, 0);
		continue;
    }
	request = *receivedRequest;
		
	if (request.type == REQUEST_EXIT) {
		
	chatShuttingDown = 1;
	
	/* wake SendThread if it is waiting for an outgoing message */
	if (waitingSender != PNUL) {
		Reply(waitingSender, NULL, 0);
		waitingSender = PNUL;
		}
	/* wake ScreenThread if it is waiting for an incoming message */
	if (waitingScreen != PNUL) {
		Reply(waitingScreen, NULL, 0);
		waitingScreen = PNUL;
	}
	
	printf("ServerThread: shutting down\n");
	
	replyStatus = Reply(senderPid, NULL, 0);
	if (replyStatus != 0 ) {
		fprintf(stderr, "ServerThread: exit reply failed\n");
	}
	continue;
	}
	
	/* worker has finished its cleanup */
	if (request.type == REQUEST_WORKER_DONE) {
		workersFinished++;
		
	
                if (chatShuttingDown && workersFinished == 4) {                 
                        printf("ServerThread: all workers stopped\n");          
                        
		
		while (ListCount(incomingQueue) > 0) {
			queuedMessage = ListFirst(incomingQueue);
			ListRemove(incomingQueue);
			free(queuedMessage);
		}
		
		while (ListCount(outgoingQueue) > 0) {
			queuedMessage = ListFirst(outgoingQueue);
			ListRemove(outgoingQueue);
			free(queuedMessage);
		}
		
		ListFree(incomingQueue, free);
		ListFree(outgoingQueue, free);
		close(chatConfig.socketFd);
		printf("ServerThread: UDP socket closed\n");
		Reply(senderPid, NULL, 0);
		return;
	}

		replyStatus = Reply(senderPid, NULL, 0);
		
		if (replyStatus != 0 ) {
			fprintf(stderr, "ServerThread: worker reply failed\n");
		}
		
	
		continue;
	}
		
	/* dont accpet new work after shutdown begins */
	if (chatShuttingDown) {
		Reply(senderPid, NULL, 0);
		continue;
	}
	

	

	/* when a worker sends a message the server receives it */
	if (request.type == ADD_OUTGOING || request.type == ADD_INCOMING) {
		newMessage = (ChatMessage *)malloc(sizeof(ChatMessage));

		if (newMessage == NULL) {
			fprintf(stderr, "ServerThread: malloc failed\n");
			Reply(senderPid, NULL, 0);
			continue;

		}
		/* the server checks whether it is incoming or outgoing */
		*newMessage = request.message;
		if (request.type == ADD_OUTGOING) {
			if (ListAppend(outgoingQueue, newMessage) != 0) {
				fprintf(stderr, "ServerThread: outgoing "
					"queue\n");
				free(newMessage);

			} else if (waitingSender != PNUL) {
		/* sender is waiting for an outgoing message
 		* remove the oldest messages and wake the sender
 		*/
		ListFirst(outgoingQueue);
		queuedMessage = (ChatMessage*)ListRemove(outgoingQueue);

		delivered = Reply(waitingSender, queuedMessage,
			sizeof(ChatMessage));
		if (delivered != 0) {
			fprintf(stderr, "ServerThread: sender reply failed\n");
			
		}

		waitingSender = PNUL;

	
 		/* the sender has finished using it
 		*/

		}

		} else {
			if (ListAppend(incomingQueue, newMessage) != 0 ) {
				fprintf(stderr, "ServerThread: incoming "
					"queue full\n");
				free(newMessage);
			}
		else if (waitingScreen != PNUL) {
		/* wake the screen thread with the oldest message */
		ListFirst(incomingQueue);
		queuedMessage = (ChatMessage*)ListRemove(incomingQueue);

		delivered = Reply(waitingScreen, queuedMessage,
		sizeof(ChatMessage));
		if(delivered != 0) {
			fprintf(stderr, "ServerThread: screen reply failed\n");
		free(queuedMessage);
		}
		waitingScreen = PNUL;
	}
	}
	replyStatus = Reply(senderPid, NULL, 0);
	
	continue;
	}

	  /* remember workers waiting for messages */
        if (request.type == REQUEST_OUTGOING) {
                if(ListCount(outgoingQueue) == 0) {
                        waitingSender = senderPid;
                        continue;
                }
		/* retrieve the oldest outgoing message */
		ListFirst(outgoingQueue);
		queuedMessage = (ChatMessage*)ListRemove(outgoingQueue);

		/* deliver it to SendThread */
		replyStatus = Reply(senderPid, queuedMessage,
		sizeof(ChatMessage));

		if(replyStatus != 0) {
		fprintf(stderr, "ServerThread outgoing reply failed\n");
		free(queuedMessage);
	  }
		continue;
	}

	  /* remember workers waiting for messages */
        if (request.type == REQUEST_INCOMING) {
                if(ListCount(incomingQueue) == 0) {
                        waitingScreen = senderPid;
                        continue;
                }
		ListFirst(incomingQueue);
		queuedMessage = (ChatMessage*)ListRemove(incomingQueue);
		replyStatus = Reply(senderPid, queuedMessage,
		sizeof(ChatMessage));

		if(replyStatus != 0) {
		fprintf(stderr, "ServerThread: incoming reply failed\n");
		free(queuedMessage);
	}
	continue;
	}

	if (replyStatus < 0) {
		fprintf(stderr, "ServerThread: Reply failed \n");
		}
	}
	
	
	
	/* Close the UDP socket after all workers have stopped */
	if (close(chatConfig.socketFd) == -1) {
		perror("ServerThread: close socket");
	} else {
		printf("ServerThread: UDP socket closed\n");
	}


	

}

