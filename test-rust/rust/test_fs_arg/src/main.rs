use std::env;
use std::fs;//::{self, File};
//use std::io::prelude::*;
//use std::io::LineWriter;

fn main() {

    let mut i_cnt = 0;
    let mut s_arg: String = "".to_string();

    // Prints each argument on a separate line
    for argument in env::args() {
        //  println!("{}", argument);
        if {
            i_cnt += 1;
            i_cnt
        } > 1
        {
            s_arg += &(argument + " ");//Собирает строку с названием входного файла (типа с пробелами может быть)
        };
    }

    println!("read file {}", s_arg);

    let s_line = match fs::read_to_string(&s_arg) {
        Err(why) => panic!("couldn't open {}: {}", s_arg, why),
        Ok(str) => str,
    };

    //println!("{}", s_line);
    let s_line: Vec<&str> = s_line.split("\r\n").collect(); // преоброзовари в массив строк


    for element in s_line.iter() {
        println!("{}", element); // выводим построчно содержимое файла
    }

}
