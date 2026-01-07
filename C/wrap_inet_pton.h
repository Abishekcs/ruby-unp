/**
 * InetPton - Wrapper function for inet_pton with error handling
 * 
 * This function converts an IP address from text (presentation) format to 
 * binary (network) format, with built-in error checking and reporting.
 * 
 * @param af_inet     Address family specification:
 *                    - AF_INET for IPv4 addresses
 *                    - AF_INET6 for IPv6 addresses
 * 
 * @param ip_address  Pointer to a null-terminated string containing the IP
 *                    address in presentation format (e.g., "192.168.1.1" for
 *                    IPv4 or "2001:db8::1" for IPv6)
 * 
 * @param sin_address Pointer to a buffer where the converted binary address
 *                    will be stored. This should point to:
 *                    - struct in_addr for AF_INET (IPv4)
 *                    - struct in6_addr for AF_INET6 (IPv6)
 * 
 * @return            Returns 1 on successful conversion. The function calls
 *                    err_quit() and terminates the program if conversion fails.
 * 
 * Error Handling:
 * - If inet_pton returns 0: The IP address format is invalid for the 
 *   specified address family
 * - If inet_pton returns -1: The address family is not supported
 * - In either error case, err_quit() is called with a descriptive message
 *   and the program terminates
 * 
 * Note: This is a convenience wrapper that eliminates the need for callers
 * to check return values, suitable for applications where invalid IP addresses
 * are considered fatal errors.
 */
int InetPton(int af_inet, const char *ip_address, void *sin_address) {
    int result = inet_pton(af_inet, ip_address, sin_address);
    if(result <= 0) {
        err_quit("inet_pton error for %s", ip_address);
    }
    return result;
}
