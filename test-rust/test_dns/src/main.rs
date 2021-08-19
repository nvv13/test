use std::net::{ SocketAddr, ToSocketAddrs };

fn main() {

    let mut server_details = "ya.ru:80".to_socket_addrs().unwrap();
    let server: SocketAddr = server_details
        .next()
        .expect("Unable to parse socket address");
    println!("{:?}", server);
    println!("{}", server.ip());
    println!("{}", server.port());

}
