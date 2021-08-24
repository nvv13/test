use std::fs::{self, File};
use std::io::prelude::*;
use std::io::LineWriter;

fn main() -> std::io::Result<()> {
    let road_not_taken = b"l,tl,l,
555
yyyu
gtgtg
.";

    let file = File::create("text1.txt")?;
    let mut file = LineWriter::new(file);

    file.write_all(b"test test")?;

    //
    assert_eq!(fs::read_to_string("text1.txt")?, "");
    file.write_all(b"\r\n")?;
    assert_eq!(fs::read_to_string("text1.txt")?, "test test\r\n",);

    println!("read_to_string {}", fs::read_to_string("text1.txt")?);
    // Write
    file.write_all(
        b"123 ghyyy
mmmm tttht
tttttttttttttttttt.",
    )?;

    file.flush()?;

    // Confirm write
    assert_eq!(fs::read("text1.txt")?, &road_not_taken[..]);
    Ok(())
}
