use std::fs::{self, File};
use std::io::prelude::*;
use std::io::LineWriter;

fn main() -> std::io::Result<()> {
    let road_not_taken = b"I shall be telling this with a sigh
Somewhere ages and ages hence:
Two roads diverged in a wood, and I -
I took the one less traveled by,
And that has made all the difference.";

    let file = File::create("poem.txt")?;
    let mut file = LineWriter::new(file);

    file.write_all(b"I shall be telling this with a sigh")?;

    // No bytes are written until a newline is encountered (or
    // the internal buffer is filled).
    assert_eq!(fs::read_to_string("poem.txt")?, "");
    file.write_all(b"\r\n")?;
    assert_eq!(
        fs::read_to_string("poem.txt")?,
        "I shall be telling this with a sigh\r\n",
    );

    println!("read_to_string {}",fs::read_to_string("poem.txt")?);
    // Write the rest of the poem.
    file.write_all(b"Somewhere ages and ages hence:
Two roads diverged in a wood, and I -
I took the one less traveled by,
And that has made all the difference.")?;

    // The last line of the poem doesn't end in a newline, so
    // we have to flush or drop the `LineWriter` to finish
    // writing.
    file.flush()?;

    // Confirm the whole poem was written.
    assert_eq!(fs::read("poem.txt")?, &road_not_taken[..]);
    Ok(())
}

