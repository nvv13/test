use std::env;
use std::fs::{self, File};
//use std::io::prelude::*;
use std::io::LineWriter;
use std::io::Write;

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
            s_arg += &(argument + " "); //Собирает строку с названием входного файла (типа с пробелами может быть)
        };
    }

    if s_arg == "" {
        s_arg = "test_padeg.bpr".to_string();
    };

    println!("read file {}", s_arg);

    let s_line = match fs::read_to_string(&s_arg) {
        Err(why) => panic!("couldn't open {}: {}", s_arg, why),
        Ok(str) => str,
    };

    //println!("{}", s_line);
    let s_line: Vec<&str> = s_line.split("\r\n").collect(); // преоброзовари в массив строк

    let s_remove: Vec<&str> = [
        "stride50",
        "rcpkg50",
        "direct50",
        "crbide50",
        "DirNotif",
        "EditCenter",
        "textLOG",
        "proide50",
        "DWheel",
        "stedit",
    ]
    .to_vec();

    let file = File::create(s_arg).unwrap();
    let mut file = LineWriter::new(file);

    for element in s_line.iter() {
        let mut s_out = element.to_string();
        for rem in s_remove.iter() {
            let s_rem = rem.to_string() + ".lib";
            s_out = s_out.replace(&s_rem, "");
            s_out = s_out.replace(&s_rem.to_lowercase(), "");
            let s_rem = rem.to_string() + ".bpi";
            s_out = s_out.replace(&s_rem, "");
            s_out = s_out.replace(&s_rem.to_lowercase(), "");
        }
        s_out = s_out + "\r\n";
        //println!("{}", s_out); // выводим построчно содержимое файла
        file.write_all(s_out.as_bytes()).unwrap();
    }
    //file.flush();
}
