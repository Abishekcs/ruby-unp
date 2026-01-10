require 'socket'

# Daytime Client - Connects to a daytime server to retrieve current time
#
# The daytime protocol (RFC 867) is a simple TCP service that returns the current
# date and time in a human-readable format. The server sends the time string
# and then closes the connection.
#
# Usage: ruby daytime_client.rb <hostname_or_IPaddress> [port]
#   hostname_or_IPaddress: The server to connect to
#   port: Optional port number (defaults to 13, the standard daytime port)
abort('usage: ruby daytime_client.rb <hostname_or_IPaddress> [port]') if ARGV.empty?

host = ARGV[0]
port = (ARGV[1] || 13).to_i # default to port 13

# Create TCP socket connection to the daytime server
begin
  socket = TCPSocket.new(host, port)

  # Read and display all lines sent by the server
  # The daytime server typically sends one line with the current time
  while line = socket.gets
    puts line
  end
rescue Errno::ECONNREFUSED
  # Handle case when server is not running or not accepting connections
  warn "Connection refused — is the server running on #{host}:#{port}?"
rescue SocketError => e
  # Handle DNS resolution errors and other socket-related issues
  warn "Socket error: #{e.message}"
ensure
  # Ensure socket is properly closed even if an error occurs
  socket.close if socket
end
