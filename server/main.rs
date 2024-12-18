use tokio::net::UdpSocket;
use std::{collections::HashMap, io, time::{SystemTime, UNIX_EPOCH}};

#[tokio::main]
async fn main() -> io::Result<()> {
    let socket = UdpSocket::bind("127.0.0.1:8080").await?;
    let mut buf = [0; 1024];
    let mut users_map = HashMap::<String, usize>::new();
    let mut last_id = 0;

    loop {
        let (len, addr) = socket.recv_from(&mut buf).await?;
        let received_data = String::from_utf8_lossy(&buf[..len]);
        println!("{:?} recevied from {:?}", received_data, addr);
        users_map.entry(addr.to_string()).or_insert(last_id);
        last_id += 1;

        for user in users_map.keys() {
            let mut data = Vec::new();
            let start = SystemTime::now();
            let since_the_epoch = start.duration_since(UNIX_EPOCH).unwrap();
            data.append(&mut since_the_epoch.as_millis().to_string().as_bytes().to_vec());
            data.push(':' as u8);
            data.append(&mut buf[..len].to_vec());
            let len = socket.send_to(&data, user).await?;
            println!("{:?} bytes sent to {}", len, user);
        }
    }
}
