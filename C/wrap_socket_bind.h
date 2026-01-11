/**
 * Bind - Wrapper function for the bind() system call with error handling
 * 
 * This function wraps the standard bind() system call to provide consistent
 * error handling across the application. It associates a socket file descriptor
 * with a specific local address and port.
 * 
 * @param fd    The socket file descriptor to bind
 * @param sa    Pointer to a sockaddr structure containing the address to bind to.
 *              This is typically cast from sockaddr_in (IPv4) or sockaddr_in6 (IPv6)
 * @param salen The size of the address structure pointed to by sa
 * 
 * @return void - Function terminates the program via err_sys() if bind fails
 * 
 * Note: This function does not return on error; instead it calls err_sys()
 *       which typically logs the error and terminates the program.
 */
void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
  // Attempt to bind the socket to the specified address
  // Returns -1 on failure, 0 on success
  if (bind(fd, sa, salen) < 0)
    err_sys("bind_error");  // Terminate with error message if bind fails
}
