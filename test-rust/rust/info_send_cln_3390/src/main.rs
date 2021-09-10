//use local_ip_address::list_afinet_netifas;
use local_ip_address::local_ip;

use std::error::Error;
use std::io;
use std::net::{SocketAddr, ToSocketAddrs};
use std::str;

use tokio::io::AsyncWriteExt;
use tokio::net::TcpStream;

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let my_local_ip = local_ip().unwrap();
    println!("это мой local IP address: {}", my_local_ip.to_string());
    /*
        let network_interfaces = list_afinet_netifas().unwrap();

        for (name, ip) in network_interfaces.iter() {
            println!("{}:\t{:?}", name, ip);
        }
    */

    // вместо "192.168.24.32:3390", может быть "ya.ru:80", и т.д.
    let mut server_details = "192.168.24.32:3390".to_socket_addrs().unwrap();
    let server: SocketAddr = server_details
        .next()
        .expect("Unable to parse socket address");
    println!("соедениться с {}:{}", server.ip(), server.port());

    let mut stream = TcpStream::connect(server).await?;

    // Write some data.
    stream.write_all(my_local_ip.to_string().as_bytes()).await?;

    loop {
        // Wait for the socket to be readable
        stream.readable().await?;

        // Creating the buffer **after** the `await` prevents it from
        // being stored in the async task.
        let mut buf = [0; 4096];

        // Try to read data, this may still fail with `WouldBlock`
        // if the readiness event is a false positive.
        match stream.try_read(&mut buf) {
            Ok(0) => break,
            Ok(n) => {
                println!("read {} bytes", n);
                let str_buf = str::from_utf8(&buf[..n]).unwrap();
                println!("{}", str_buf);
                break; // считали, и все, пора отсоеденятся
            }
            Err(ref e) if e.kind() == io::ErrorKind::WouldBlock => {
                continue;
            }
            Err(e) => {
                return Err(e.into());
            }
        }
    }

    Ok(())
}
