require 'socket'
require 'time'

# Daytime Server - Implements the RFC 867 daytime protocol
#
# The daytime protocol is a simple TCP service that returns the current
# date and time in a human-readable format. Once a client connects,
# the server sends a single line containing the current time and then
# closes the connection.
#
# This server uses port 13013 (non-privileged) instead of the standard
# port 13 to avoid requiring root privileges.

PORT = 13_013 # use a non-privileged port

# Create TCP server bound to all interfaces (0.0.0.0)
server = TCPServer.new('0.0.0.0', PORT)
puts "Daytime server running on port #{PORT}..."

# Main server loop - handle incoming connections indefinitely
loop do
  # Accept incoming client connection (blocking call)
  client = server.accept

  # Get client connection information
  # peeraddr returns: [address_family, port, hostname, ip_address]
  addr = client.peeraddr
  host = addr[2]  # hostname
  ip   = addr[3]  # IP address

  # Log the incoming connection with timestamp
  puts "[#{Time.now.strftime('%H:%M:%S')}] Connection from #{host} (#{ip})"

  # Format current time according to RFC 867 standard
  # Format: "Day Month Date HH:MM:SS YYYY\r\n"
  time_str = Time.now.strftime("%a %b %d %H:%M:%S %Y\r\n")

  # Send the time string to the client
  client.write(time_str)

  # Log what was sent for debugging/monitoring
  puts "[#{Time.now.strftime('%H:%M:%S')}] Sent time '#{time_str.strip}' to #{ip}"

  # Close the client connection (daytime protocol sends once then closes)
  client.close
end
