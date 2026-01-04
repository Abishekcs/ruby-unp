#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>   // for variadic functions (used by error handling)
// ------------------------------------
// Custom wrapper headers for error handling and socket operations
#include "err.h"                  // Error handling functions (err_quit, err_sys)
#include "wrapsock.h"             // Socket wrapper functions with error checking
#include "wrap_inet_pton.h"       // Wrapper for inet_pton with error handling
#include "wrap_socket_connect.h"  // Wrapper for socket connect with error handling

#define MAXLINE 4096              // Maximum line length for receiving data
#define SA struct sockaddr        // Shorthand for struct sockaddr casting

/**
 * TCP Daytime Client
 * 
 * This program implements a simple TCP client that connects to a daytime server
 * (RFC 867) which listens on port 13. The daytime protocol is a simple service
 * that returns the current date and time as a character string.
 * 
 * Flow:
 * 1. Validate command-line arguments
 * 2. Create a TCP socket
 * 3. Configure server address structure
 * 4. Connect to the daytime server
 * 5. Read the server's response
 * 6. Display the response to stdout
 * 7. Close and exit
 * 
 * Usage: ./daytimetcpcli <IPaddress> <port>
 * Example: ./daytimetcpcli 192.168.1.100 13
 */
int main(int argc, char **argv)
{
    int sockfd, n;                    // sockfd: socket file descriptor
                                      // n: number of bytes read from socket
    char recvline[MAXLINE + 1];       // Buffer to store received data (+1 for null terminator)
    struct sockaddr_in servaddr;      // Server address structure for IPv4
    
    // Validate command-line arguments
    // argc should be 3: program name, IP address, and port (though port isn't used yet - see bug note)
    if (argc != 3)
      err_quit("usage: %s <IPaddress>", argv[1]);
 
    // Create a TCP socket
    // AF_INET: IPv4 Internet protocols
    // SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte streams
    // 0: Protocol (0 means choose the default protocol for the socket type - TCP for SOCK_STREAM)
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    // Initialize server address structure to zero
    // This ensures all fields start with a clean state
    bzero(&servaddr, sizeof(servaddr));
    
    // Configure server address structure
    servaddr.sin_family = AF_INET;    // Address family: IPv4
    servaddr.sin_port   = htons(13);  // Port number: 13 (daytime server)
                                      // htons: Host TO Network Short - converts port to network byte order
    
    // Convert IP address from text (presentation format) to binary (network format)
    // argv[1]: IP address as string (e.g., "192.168.1.100")
    // &servaddr.sin_addr: Destination for binary IP address
    InetPton(AF_INET, argv[1], &servaddr.sin_addr);
    
    // Establish connection to the server
    // sockfd: Socket file descriptor
    // (SA *) &servaddr: Cast sockaddr_in to generic sockaddr pointer
    // sizeof(servaddr): Size of the address structure
    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    
    // Read data from server in a loop
    // The daytime server typically sends one response and closes, but we use a loop
    // to handle any amount of data the server might send
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        // n > 0: Successfully read n bytes
        
        recvline[n] = 0;  // Null-terminate the string to make it a valid C string
                          // This is crucial because read() doesn't add null terminator
        
        // Write the received string to stdout
        // fputs: File PUT String - writes string to specified stream
        if (fputs(recvline, stdout) == EOF)
            err_sys("fputs error");  // Exit with error if write to stdout fails
    }
    
    // Check if read() encountered an error
    // n < 0: Error occurred during read
    // n == 0: Server closed connection (normal EOF) - this is expected behavior
    if (n < 0)
        err_sys("read error");  // Exit with system error message
    
    // Clean exit
    // Note: In production code, should close(sockfd) before exit
    // The OS will clean up the socket, but explicit cleanup is better practice
    exit(0);
}
