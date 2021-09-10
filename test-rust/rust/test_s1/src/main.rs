fn main() {
    let s1 = String::from("hello");
    let (s1, len) = calculate_length(s1);
    println!("1 Длина '{}' равна {}.", s1, len);

    let (s2, len) = calculate_length(s1);
    println!("2 Длина '{}' равна {}.", s2, len);

    println!("3 Длина '{}' равна {}.", s1, len);

}

fn calculate_length(mut s: String) -> (String, usize) {
    s += " 123";
    let length = s.len(); // len() возвращает длину экземпляра типа String
    (s, length)
}
