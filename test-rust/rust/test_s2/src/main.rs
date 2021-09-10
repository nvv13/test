fn main() {
    let s1 = String::from("hello");
    let len = calculate_length(&s1);
    println!("Длина '{}' равна {}.", s1, len);
}

fn calculate_length(s: &String) -> usize {
    let length = s.len(); // len() возвращает длину экземпляра типа String
    length
}
