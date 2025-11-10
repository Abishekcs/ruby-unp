int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen) {
  int response;

  response = connect(sockfd, servaddr, addrlen);
  
  if (response < 0)
    err_sys("connect error");
  
  return response;
}
