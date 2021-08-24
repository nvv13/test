use std::env;

fn main() {
    println!("start");
// Prints each argument on a separate line
for argument in env::args() {
    println!("{}", argument);
}

}
