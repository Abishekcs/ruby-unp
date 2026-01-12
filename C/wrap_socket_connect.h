/**
 * Connect - Wrapper function for connect() with error handling
 * @sockfd: Socket file descriptor to connect
 * @servaddr: Pointer to sockaddr structure containing server address
 * @addrlen: Length of the address structure
 *
 * Description:
 * This function wraps the standard connect() system call and provides
 * automatic error checking. It attempts to establish a connection on
 * the specified socket to the server address provided.
 *
 * Return:
 * On success, returns the result from connect() (typically 0).
 * On failure, calls err_sys() which prints an error message and
 * terminates the process.
 *
 * Note:
 * Function name is capitalized to distinguish it from the standard
 * connect() system call while maintaining a similar interface.
 */
int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen) {
  int response;
  
  /* Attempt to connect the socket to the server address */
  response = connect(sockfd, servaddr, addrlen);
  
  /* Check for connection failure (negative return value indicates error) */
  if (response < 0)
    err_sys("connect error");  /* Print error and terminate process */
  
  /* Return the connect() result on success */
  return response;
}
