require 'socket'

# Usage: ruby daytime_client.rb <hostname_or_IPaddress> [port]
if ARGV.empty?
  abort("usage: ruby daytime_client.rb <hostname_or_IPaddress> [port]")
end

host = ARGV[0]
port = (ARGV[1] || 13).to_i  # default to port 13

begin
  socket = TCPSocket.new(host, port)
  while line = socket.gets
    puts line
  end
rescue Errno::ECONNREFUSED
  warn "Connection refused — is the server running on #{host}:#{port}?"
rescue SocketError => e
  warn "Socket error: #{e.message}"
ensure
  socket.close if socket
end

