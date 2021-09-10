fn main() {
    //       0  1  2  3  4
    let a = [1, 2, 3, 4, 5];
    let slice = &a[1..3]; // = 2, 3 

    for (i, &item) in slice.iter().enumerate() {
        println!("i={} value={}", i, &item);
    }
}
