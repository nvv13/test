mod my {
    // для того, чтобы использовать private функции, применяем mod

    use rand::prelude::*;

    #[derive(Debug)]
    pub struct Listopad {
        list: [u8; 10], // массив u8
    }

    impl Listopad {
        fn rand(&mut self) {
            for x in &mut self.list {
                *x = random();
            }
        }

        pub fn new_rand() -> Listopad {
            let mut new1 = Listopad { list: [0; 10] };
            new1.rand();
            new1
        }

        //---- Проверка упорядоченности массива
        pub fn is_sorted(&mut self) -> bool {
            for i in 0..self.list.len() - 1 {
                if self.list[i] > self.list[i + 1] {
                    return false;
                };
            }
            return true;
        }

        // сортирует, просто перебором всех элементов, и перемещением меньшего в начало
        pub fn sort(&mut self) {
            for i_pos in 0..self.list.len() - 1 {
                let mut min_i = i_pos;
                let mut val_i = self.list[i_pos];
                for i in i_pos..self.list.len() {
                    if val_i > self.list[i] {
                        min_i = i;
                        val_i = self.list[i];
                    };
                }
                self.list[min_i] = self.list[i_pos];
                self.list[i_pos] = val_i;
            }
        }
    }
}

fn main() {
    // 1) создать мессив с произвольными данными
    let mut a1 = my::Listopad::new_rand();

    // 2) выводим
    println!("a1 равен {:#?}", a1);

    // 3) сортируем
    a1.sort();

    // 4) выводим, все
    println!("a1.sort() равен {:#?}", a1);

    // 5) проверка, отсортировано ли
    println!("a1.is_sorted() равен {:#?}", a1.is_sorted());
}
