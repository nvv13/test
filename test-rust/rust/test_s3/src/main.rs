fn main() {
    let mut s1 = String::from("hello");
    let len = calculate_length(&mut s1);
    println!("Длина '{}' равна {}.", s1, len);
}

fn calculate_length(s: &mut String) -> usize {
    s.push_str(" 123");
    *s+=" 123";
    let length = s.len(); // len() возвращает длину экземпляра типа String
    length
}
