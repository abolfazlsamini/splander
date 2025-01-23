use tokio::net::UdpSocket;  
use std::{collections::HashMap, io};  

struct Player {  
    id: usize,  
    name: String, //Added player name  
    addr: String,  
}  

struct Lobby {  
    players: HashMap<usize, Player>,  
    next_id: usize,  
}  

impl Lobby {  
    fn new() -> Self {  
        Lobby { players: HashMap::new(), next_id: 0 }  
    }  

    fn add_player(&mut self, name: String, addr: String) {  
        let id = self.next_id;  
        self.next_id += 1;  
        self.players.insert(id, Player { id, name, addr });  
        
    }  

    fn remove_player(&mut self, id: usize) {  
        self.players.remove(&id);  
    }  

    fn get_player_list(&self) -> String {  
        self.players.iter()  
            .map(|(&id, player)| format!("{}:{} ", player.name, id))  
            .collect::<Vec<_>>()  
            .join("")  
    }  

    fn get_player_by_addr(&self, addr: &String) -> Option<&Player> {  
        self.players.values().find(|p| p.addr == *addr)  
    }  
}  

fn who_is_in_lobby(){

}

#[tokio::main]  
async fn main() -> io::Result<()> {  
    let socket = UdpSocket::bind("127.0.0.1:8080").await?;  
    let mut buf = [0; 1024];  
    let mut lobby = Lobby::new();  

    loop {  
        let (len, addr) = socket.recv_from(&mut buf).await?;  
        let received_data = String::from_utf8_lossy(&buf[..len]);  
        println!("{:?} received from {:?}", received_data, addr);  

        let addr_str = addr.to_string();  

        //New player. Add them to the lobby  
        let parts: Vec<&str> = received_data.split(':').collect();  
        if parts[0] == "name" {  
            let name = parts[1].to_string();
            lobby.add_player(name.clone(), addr_str.clone());  
            let player_list = lobby.get_player_list();

            let response = format!("Welcome {}! Players: {}", name, player_list);  
            socket.send_to(response.as_bytes(), &addr).await?;  

            //Broadcast new player information
            for (_, player) in lobby.players.iter() {  
                let broadcast_msg = format!("{} joined lobby. Players: {}", name, player_list);  
                socket.send_to(broadcast_msg.as_bytes(), &player.addr).await?;  
            }
        } else {  
            println!("lobby players: {:?}. recieved data: {:?}", lobby.get_player_list(), received_data);  
        }  
    }  
}