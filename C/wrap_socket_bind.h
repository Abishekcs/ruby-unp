void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
  if (bind(fd, sa, salen) < 0)
    err_sys("bind_error");
}
