int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int n;  
    /* Variable to store the file descriptor returned by accept() */

again:
    /* Label: execution jumps here if we need to retry accept() */

    n = accept(fd, sa, salenptr);
    /* accept() waits for an incoming connection and returns a new
       connected socket file descriptor, or -1 on error */

    if (n < 0) {
        /* Check if accept() failed */

#ifdef EPROTO
        /* EPROTO may be defined on some UNIX systems to indicate
           a protocol-related error during connection setup */

        if (errno == EPROTO || errno == ECONNABORTED)
            /* If the connection was aborted before it was accepted,
               this is a transient error, not a fatal one */
            goto again;
            /* Retry accept() to wait for the next connection */
#else
        /* On systems without EPROTO, only check ECONNABORTED */

        if (errno == ECONNABORTED)
            /* The client aborted the connection before accept() */
            goto again;
            /* Retry accept() */
#endif

        /* Any other error means something serious went wrong */
        err_sys("accept error");
        /* Print error message and terminate the process */
    }

    return n;
    /* Return the connected socket descriptor to the caller */
}

