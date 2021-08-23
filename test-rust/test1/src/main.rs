use std::error::Error;
use std::io;
use tokio::io::AsyncWriteExt;
use tokio::net::TcpStream;

//use tokio::io::AsyncReadExt;
use std::str;

use std::net::{SocketAddr, ToSocketAddrs};

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let mut server_details = "www.pogodaiklimat.ru:80".to_socket_addrs().unwrap();
    let server: SocketAddr = server_details
        .next()
        .expect("Unable to parse socket address");
    println!("{:?}", server);
    println!("{}", server.ip());
    println!("{}", server.port());

    //let mut stream = TcpStream::connect("95.30.236.187:8181").await?;
    let mut stream = TcpStream::connect(server).await?;

    // Write some data.
    stream
        .write_all(
            b"GET // HTTP//1.1\r\n
Host: www.pogodaiklimat.ru\r\n
Connection: keep-alive\r\n
User-Agent: Mozilla//5.0\r\n
Accept: *//*\r\n
Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7",
        )
        .await?;

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
                //println!("{:?}",&buf[..n]);
                let str_buf = str::from_utf8(&buf[..n]).unwrap();
                println!("{}", str_buf);
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
