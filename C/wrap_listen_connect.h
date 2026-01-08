/**
 * Listen - Wrapper function for the listen() system call
 * 
 * @fd: File descriptor of the socket to listen on
 * @backlog: Maximum length of the pending connections queue
 * 
 * This function wraps the listen() system call with additional functionality:
 * - Allows overriding the backlog parameter via the LISTENQ environment variable
 * - Provides error handling with a descriptive error message
 */
void Listen(int fd, int backlog) {
    char *ptr;
    
    // Check if LISTENQ environment variable is set to override backlog
    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);
    
    // Call listen() and handle errors
    if (listen(fd, backlog) < 0)
        err_sys("listen error");
}
