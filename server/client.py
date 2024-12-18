import socket

message = "Hello!!"
bytes_to_send = str.encode(message)
server_addr_port = ("127.0.0.1", 8080)
buffer_size = 1024

udp_client_scoket = socket.socket(
        family=socket.AF_INET, type=socket.SOCK_DGRAM)

udp_client_scoket.sendto(bytes_to_send, server_addr_port)

msg_from_server = udp_client_scoket.recvfrom(buffer_size)

print(f"server: {msg_from_server[0]}")

while True:
    sendmsg = str.encode(input())
    udp_client_scoket.sendto(sendmsg, server_addr_port)
    print(f"server: {udp_client_scoket.recvfrom(buffer_size)[0]}")
    