require 'socket'
require 'time'

PORT = 13013  # use a non-privileged port

server = TCPServer.new('0.0.0.0', PORT)
puts "Daytime server running on port #{PORT}..."

loop do
  client = server.accept
  addr = client.peeraddr  # [address_family, port, hostname, ip_address]
  host = addr[2]
  ip   = addr[3]

  puts "[#{Time.now.strftime('%H:%M:%S')}] Connection from #{host} (#{ip})"

  time_str = Time.now.strftime("%a %b %d %H:%M:%S %Y\r\n")
  client.write(time_str)

  puts "[#{Time.now.strftime('%H:%M:%S')}] Sent time '#{time_str.strip}' to #{ip}"

  client.close
end

