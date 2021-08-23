
install from fedora linux

и сделать по

https://www.rust-lang.org/tools/install

# cd ~
# curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

создаст все дирикторий, и начальные файлы
cargo new имя_проекта

ну, тут наверно все понятно
cargo check  - для скорости, проверит - соберёться ли... , быстрее чем build, run

cargo build

для его компиляции с оптимизациями.
cargo build --release 

cargo run

Cargo.lock - файл, формируеться автоматом, собирает зависимости... можено поглядеть , если что

"очистка"
cargo clean

форматировать исходник
cargo fmt

сделать док по текущему проекту, и открыть в браузере!!!
cargo doc --open

