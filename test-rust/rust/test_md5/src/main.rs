// src/main.rs

use std::env;
use std::fs;
use std::io;

fn main() -> io::Result<()> {
    // 1. Проверка аргументов командной строки
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        eprintln!("Usage: {} <file_name>", args[0]);
        return Ok(());
    }

    let filename = &args[1];

    println!("Reading file: {}", filename);

    // 2. Чтение всего содержимого файла в вектор байтов
    let file_contents = match fs::read(filename) {
        Ok(bytes) => bytes,
        Err(e) => {
            eprintln!("Error reading file {}: {}", filename, e);
            return Err(e);
        }
    };

    // 3. Вычисление MD5 (ИСПРАВЛЕНО ЗДЕСЬ)
    // Используем md5::compute, как рекомендовано компилятором.
    let hash_result = md5::compute(&file_contents);

    // 4. Форматирование и вывод результата
    // Преобразуем массив байтов в шестнадцатеричную строку для удобства чтения
    let hex_digest = hash_result.iter()
        .map(|b| format!("{:02x}", b))
        .collect::<String>();

    println!("\n--- MD5 Hash ---");
    println!("{}", hex_digest);
    println!("----------------");

    Ok(())
}
