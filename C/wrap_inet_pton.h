int InetPton(int af_inet, const char *ip_address, void *sin_address) {
  int result = inet_pton(af_inet, ip_address, sin_address);
  if(result <=0) {
    err_quit("inet_pton error for %s", ip_address);
  }
  return result;
}
