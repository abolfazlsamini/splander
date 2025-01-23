import socket
import time
import sys

msgs1 = ["1", "2", "3", "4"]
msgs2 = ["CAT", "DOG", "ZIBRA", "BIRD"]

if len(sys.argv) > 1:
    if sys.argv[1] == "1":
        msgs = msgs1
    else:
        msgs = msgs2

server_addr_port = ("127.0.0.1", 8080)
buffer_size = 1024

udp_client_scoket = socket.socket(
        family=socket.AF_INET, type=socket.SOCK_DGRAM)

index = 0
bytes_to_send = str.encode("INITIAL")
udp_client_scoket.sendto(bytes_to_send, server_addr_port)
while True:
    if len(sys.argv) > 1:
        msg = msgs[index]
        index += 1
        if index > 3:
            index = 0

        time.sleep(1)

        bytes_to_send = str.encode(msg)
        udp_client_scoket.sendto(bytes_to_send, server_addr_port)

    msg_from_server = udp_client_scoket.recvfrom(buffer_size)

    print(f"server: {msg_from_server[0]}")
