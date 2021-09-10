enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

impl Message {
    fn call(&self) {
        // здесь будет определено тело метода
    }
}

fn main() {
    let m = Message::Write(String::from("hello"));
    m.call();

    let x: i8 = 5;
    let y: Option<i8> = Some(5);
    let sum = x + y.unwrap();

    println!("sum = {}", sum);
}
