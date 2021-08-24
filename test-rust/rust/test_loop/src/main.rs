fn main() {
    let mut a1: i32 = 0;
    loop {
        println!("Hello, world! {}", a1);
        if {
            a1 = a1 + 1;
            a1
        } > 10
        {
            break;
        };
    }

    let mut counter = 0;
    let result = loop {
        counter += 1;
        if counter == 10 {
            break counter * 2; // у break возвращаемое значение есть! во как...
        }
    };
    println!("Результат равен {}", result);

    let mut number = 3;
    while number != 0 {
        println!("{}!", number);
        number = number - 1;
    }
    println!("Поехали!!!");

    let a = [10, 20, 30, 40, 50];
    let mut index = 0;
    while index < a.len() {
        println!("while Значение равно {}", a[index]);
        index = index + 1;
    }

    for element in a.iter() {
        println!("for Значение равно {}", element);
    }

    for number in (1..4).rev() {
        println!("{}!", number);
    }
    println!("Поехали!!! 2");

    for element in a.iter().rev() {
        println!("for rev Значение равно {}", element);
    }

}
