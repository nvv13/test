use tokio::net::TcpStream;
use std::error::Error;
use std::io;
use tokio::io::AsyncWriteExt;


#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    // Connect to a peer
    let mut stream = TcpStream::connect("95.30.236.187:8181").await?;

    // Write some data.
    stream.write_all(b"hello world!").await?;

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
                println!("{:?}",buf);
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