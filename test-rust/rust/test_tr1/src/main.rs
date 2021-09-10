#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    fn square(size: u32) -> Rectangle {
        //не имеет &self, используется для конструктора квадрата,
        //вызываеться отдельно, можно таккую "левую" функцию сделать...
        Rectangle {
            width: size,
            height: size,
        }
    }

    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
}

#[derive(Debug)]
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
    M2(Rectangle),
}

impl Message {
    fn call(&self) {
        println!("self равен {:#?}", self);
    }
}

fn main() {
    let rect1 = Rectangle {
        width: 30,
        height: 50,
    };
    println!("rect1 равен {:?}", rect1);

    println!("rect1 равен {:#?}", rect1);

    println!(
        "Площадь прямоугольника равна {} квадратным пикселам.",
        rect1.area()
    );

    let rect2 = Rectangle {
        width: 10,
        height: 40,
    };
    let rect3 = Rectangle {
        width: 60,
        height: 45,
    };

    println!("rect2 равен {:#?}", rect2);
    println!(
        "Может ли rect1 содержать в себе rect2? {}",
        rect1.can_hold(&rect2)
    );

    println!("rect3 равен {:#?}", rect3);
    println!(
        "Может ли rect1 содержать в себе rect3? {}",
        rect1.can_hold(&rect3)
    );

    let sq = Rectangle::square(3);
    println!("sq равен {:#?}", sq);

    let m1 = Message::M2(sq);
    m1.call();
}
