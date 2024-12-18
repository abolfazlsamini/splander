use tokio::net::UdpSocket;
use std::io;

#[tokio::main]
async fn main() -> io::Result<()> {
    let socket = UdpSocket::bind("127.0.0.1:8080").await?;
    let mut buf = [0; 1024];

    loop {
        let (len, addr) = socket.recv_from(&mut buf).await?;
        let received_data = String::from_utf8_lossy(&buf[..len]);
        println!("{:?} recevied from {:?}",received_data , addr);

        let len = socket.send_to(&buf[..len], addr).await?;
        println!("{:?} bytes sent", len);
    }
}
