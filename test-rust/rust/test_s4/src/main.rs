fn main() {
    let s = String::from("hello");
    let len = s.len();

    let slice = &s[3..len];
    println!("{}", slice);

    let slice = &s[3..];
    println!("{}", slice);

    let slice = &s[0..len];
    println!("{}", slice);

    let slice = &s[..];
    println!("{}", slice);
}

