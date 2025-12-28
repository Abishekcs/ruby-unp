#include <sys/types.h>

ssize_t Write(int fd, const void *ptr, size_t nbytes) {
  ssize_t n;

  if((n = write(fd, ptr, nbytes)) != (ssize_t)nbytes )
    err_sys("write error");
  return n;
}
