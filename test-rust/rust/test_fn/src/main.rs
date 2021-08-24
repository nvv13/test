fn main() {
    let condition = true;
    let number = if condition { 5 } else { 6 };
    println!("Значение числа равно {}", number);

    /* комментарий еще такой */
    println!("Значение another_function равно {}", another_function(5, 6));
    println!("Значение another_function равно {}", another_function(9, 3));
    println!(
        "Значение another_function равно {}",
        another_function(15, 6)
    );

    //let x = 5;
    let y = {
        let x = 3;
        x + 1
    };
    println!("Значение y равно {}", y);
}

fn another_function(x: i32, y: i32) -> i32 {
    println!("Значение x равно {}", x);
    println!("Значение y равно {}", y);
    if x > 10 {
        return x + y;
    } else if x > y && y > 0 {
        return x / y;
    };

    x * y
}
