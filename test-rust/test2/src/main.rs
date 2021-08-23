fn main() {
    println!("Hello, world!");

    // деление
    let quotient = 56.7 / 32.2;
    println!("quotient {}", quotient);

    let remainder = 43 / 5;
    println!("43 / 5 = {}", remainder);

    // остаток
    let remainder = 43 % 5;
    println!("43 % 5 = {}", remainder);

    println!(" {}, {}", 43 / 5, 43 % 5);

    let remainder: f64 = 43 as f64 / 5.0000;
    println!("f64 43 / 5 = {}", remainder);

    let remainder: i32 = 43;
    let remainder: f64 = remainder as f64 / 5 as f64;
    println!("f64 43 / 5 = {}", remainder);

    // Тип char равен четырем байтам, Юникод
    let c: char = 'z';
    let z: char = 'ƶ';
    let snowman: char = '☃';
    println!("c = {}", c);
    println!("z = {}", z);
    println!("snowman = {}", snowman);

    //кортеж,
    let tup: (i16, f32, u8) = (500, 6.4, 1);
    let (x, y, z) = tup;
    println!("z, y, x = {2}, {1}, {0}", x, y, z);
    println!("tup.0, tup.1, tup.2 = {}, {}, {}", tup.0, tup.1, tup.2);
    println!("tup = {:?}", tup);

    //Массив
    let a = [1, 2, 3, 4, 5];
    println!("a = {:?}", a);

    //указали тип переменных массива
    let a: [i32; 5] = [1, 2, 3, 4, 5];
    println!("a = {:?}", a);

    let months = [
        "Январь",
        "Февраль",
        "Март",
        "Апрель",
        "Май",
        "Июнь",
        "Июль",
        "Август",
        "Сентябрь",
        "Октябрь",
        "Ноябрь",
        " Декабрь",
    ];
    println!("months = {:?}", months);
    println!("months[0] = {}", months[0]);

    //массив из 5 элементов 3
    let a = [3; 5];
    println!("a = {:?}", a);

    a1();
}

fn a1() {
    println!("new");
}
