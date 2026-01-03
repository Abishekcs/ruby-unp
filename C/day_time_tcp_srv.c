#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
//---------------------
#include "err.h"
#include "wrapsock.h"
#include "wrap_socket_bind.h"
#include "wrap_listen_connect.h"
#include "wrap_accept.h"
#include "wrap_write.h"
#include "wrap_close.h"
//---------------------
#define MAXLINE 4096    // Maximum line length for buffer
#define LISTENQ 1024    // Maximum number of client connections to queue
#define SA struct sockaddr  // Shorthand for struct sockaddr type casting

/*
 * Simple Daytime TCP Server
 * 
 * This server implements a basic daytime protocol (RFC 867) that:
 * 1. Listens on port 13 (standard daytime port)
 * 2. Accepts incoming client connections
 * 3. Sends the current server time to each client
 * 4. Closes the connection immediately after sending
 * 
 * The server uses wrapper functions (Socket, Bind, Listen, etc.) which
 * likely include error handling around the standard socket API calls.
 */
int main(int argc, char **argv) {
	int					listenfd, connfd;  // File descriptors: listenfd for listening socket, connfd for connected client
	struct sockaddr_in	servaddr;          // Server address structure to hold IP and port information
	char				buff[MAXLINE];     // Buffer to store the message sent to clients
	time_t				ticks;             // Variable to store current time in seconds since epoch

	// Create a TCP socket (AF_INET = IPv4, SOCK_STREAM = TCP)
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	// Zero out the server address structure to ensure clean initialization
	bzero(&servaddr, sizeof(servaddr));
	
	// Configure server address structure
	servaddr.sin_family      = AF_INET;           // Use IPv4 address family
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections on any network interface (0.0.0.0)
	servaddr.sin_port        = htons(13);	      // Bind to port 13 (daytime server standard port)
	                                               // htons() converts port to network byte order (big-endian)

	// Bind the socket to the specified address and port
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	// Mark the socket as a passive socket that will accept incoming connections
	// LISTENQ specifies the maximum length of the queue of pending connections
	Listen(listenfd, LISTENQ);

	// Infinite loop to continuously accept and service client connections
	for ( ; ; ) {
		// Accept a new client connection (blocks until a client connects)
		// NULL parameters mean we don't care about the client's address information
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		// Get current system time in seconds since Unix epoch (Jan 1, 1970)
		ticks = time(NULL);
		
		// Format the message with current time
		// ctime() converts time_t to human-readable string format (e.g., "Fri Jan  3 21:14:22 2026\n")
		// %.24s limits the string to 24 characters (excludes the newline that ctime adds)
		snprintf(buff, sizeof(buff), "Hello From Server %.24s\r\n", ctime(&ticks));
		
		// Send the formatted message to the connected client
		Write(connfd, buff, strlen(buff));

		// Close the connection with this client
		// The server follows a connect-send-close pattern for each client
		Close(connfd);
	}
	// Note: This infinite loop means the program never reaches here
	// In production, you'd want signal handlers for graceful shutdown
}
