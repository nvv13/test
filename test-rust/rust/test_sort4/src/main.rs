use std::time::Instant;

mod my {
    // для того, чтобы использовать private функции, применяем mod

    use rand::prelude::*;

    const LEN_ARRAY: usize = 10000;

    #[derive(Debug)]
    pub struct Listopad {
        list: [i32; LEN_ARRAY], // массив i32
    }

    impl Listopad {
        fn rand(&mut self) {
            for x in &mut self.list {
                *x = random();
            }
        }

        pub fn new_rand() -> Listopad {
            let mut new1 = Listopad {
                list: [0; LEN_ARRAY],
            };
            new1.rand();
            new1
        }

        pub fn new(a1: &Listopad) -> Listopad {
            let new1 = Listopad {
                list: a1.list.clone(),
            };
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
        pub fn sort_dump(&mut self) {
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

        /*
        Сортировка методом пузырька

        Задача
        При работе с массивами данных не редко возникает задача их сортировки по возрастанию или убыванию, т.е. упорядочивания. Это значит, что элементы того же массива нужно расположить строго по порядку. Например, в случае сортировки по возрастанию предшествующий элемент должен быть меньше последующего (или равен ему).

        Решение
        Существует множество методов сортировки. Одни из них являются более эффективными, другие – проще для понимания. Достаточно простой для понимания является сортировкаметодом пузырька, который также называют методом простого обмена. В чем же он заключается, и почему у него такое странное название: "метод пузырька"?

        Как известно воздух легче воды, поэтому пузырьки воздуха всплывают. Это просто аналогия. В сортировке методом пузырька по возрастанию более легкие (с меньшим значением) элементы постепенно "всплывают" в начало массива, а более тяжелые друг за другом опускаются на дно (в конец массива).

        Алгоритм и особенности этой сортировки таковы:
         При первом проходе по массиву элементы попарно сравниваются между собой: первый со вторым, затем второй с третьим, следом третий с четвертым и т.д. Если предшествующий элемент оказывается больше последующего, то их меняют местами.
         Не трудно догадаться, что постепенно самое большое число оказывается последним. Остальная часть массива остается не отсортированной, хотя некоторое перемещение элементов с меньшим значением в начало массива наблюдается.
         При втором проходе незачем сравнивать последний элемент с предпоследним. Последний элемент уже стоит на своем месте. Значит, число сравнений будет на одно меньше.
         На третьем проходе уже не надо сравнивать предпоследний и третий элемент с конца. Поэтому число сравнений будет на два меньше, чем при первом проходе.
         В конце концов, при проходе по массиву, когда остаются только два элемента, которые надо сравнить, выполняется только одно сравнение.
         После этого первый элемент не с чем сравнивать, и, следовательно, последний проход по массиву не нужен. Другими словами, количество проходов по массиву равно m-1, где m – это количество элементов массива.
         Количество сравнений в каждом проходе равно m-i, где i – это номер прохода по массиву (первый, второй, третий и т.д.).
         При обмене элементов массива обычно используется "буферная" (третья) переменная, куда временно помещается значение одного из элементов.
        */
        pub fn sort_bubl(&mut self) {
            for i_pos in 0..self.list.len() - 1 {
                for i in 0..self.list.len() - i_pos - 1 {
                    if self.list[i] > self.list[i + 1] {
                        let val = self.list[i + 1];
                        self.list[i + 1] = self.list[i];
                        self.list[i] = val;
                    }
                }
            }
        } // end sort_bubl

        /*
         "быстрая" сортировка
        QuickSort является существенно улучшенным вариантом алгоритма сортировки с помощью прямого обмена (его варианты известны как «Пузырьковая сортировка» и «Шейкерная сортировка»), известного в том числе своей низкой эффективностью. Принципиальное отличие состоит в том, что в первую очередь производятся перестановки на наибольшем возможном расстоянии и после каждого прохода элементы делятся на две независимые группы (таким образом улучшение самого неэффективного прямого метода сортировки дало в результате один из наиболее эффективных улучшенных методов).

        Общая идея алгоритма состоит в следующем:

        Выбрать из массива элемент, называемый опорным. Это может быть любой из элементов массива. От выбора опорного элемента не зависит корректность алгоритма, но в отдельных случаях может сильно зависеть его эффективность (см. ниже).
        Сравнить все остальные элементы с опорным и переставить их в массиве так, чтобы разбить массив на три непрерывных отрезка, следующих друг за другом: «элементы меньшие опорного», «равные» и «большие»[1].
        Для отрезков «меньших» и «больших» значений выполнить рекурсивно ту же последовательность операций, если длина отрезка больше единицы.
        На практике массив обычно делят не на три, а на две части: например, «меньшие опорного» и «равные и большие»; такой подход в общем случае эффективнее, так как упрощает алгоритм разделения (см. ниже).

        */
        fn partition(&mut self, mut low: usize, mut high: usize) -> usize {
            //переставляем край, пока, слева не будутменьшие, справа большие,
            // и на выхлоп - новая позиция "края"
            loop {
                if self.list[low] > self.list[high]
                //первый элемент больше чем последний
                {
                    if low == high - 1 {
                        // тут просто - поменять местами
                        let p1 = self.list[high];
                        self.list[high] = self.list[low];
                        self.list[low] = p1;
                    } else {
                        // тут больший поместим в конец, а эталон - на одну вниз
                        let p1 = self.list[high - 1];
                        self.list[high - 1] = self.list[high];
                        self.list[high] = self.list[low];
                        self.list[low] = p1;
                    }
                    high -= 1;
                } else {
                    low += 1;
                };

                if low == high {
                    break;
                };
            }
            high
        }

        fn quicksort(&mut self, low: usize, high: usize) {
            if low < high {
                let p = self.partition(low, high);
                if p > 1 {
                    self.quicksort(low, p - 1);
                };
                if p < high + 1 {
                    self.quicksort(p + 1, high);
                };
            };
        }

        pub fn quick_sort(&mut self) {
            self.quicksort(0, self.list.len() - 1);
        } // end quick_sort
    }
}

fn main() {
    // 1) создать мессив с произвольными данными
    let a_new = my::Listopad::new_rand();

    let mut a1 = my::Listopad::new(&a_new);
    let now = Instant::now();
    a1.sort_dump();
    println!("a1.sort_dump = {:?}", now.elapsed());
    println!("a1.is_sorted() равен {:#?}", a1.is_sorted());

    let mut a1 = my::Listopad::new(&a_new);
    let now = Instant::now();
    a1.sort_bubl();
    println!("a1.sort_bubl = {:?}", now.elapsed());
    println!("a1.is_sorted() равен {:#?}", a1.is_sorted());

    let mut a1 = my::Listopad::new(&a_new);
    //println!("a1 равен {:#?}", a1);
    let now = Instant::now();
    a1.quick_sort();
    println!("a1.quick_sort = {:?}", now.elapsed());
    println!("a1.is_sorted() равен {:#?}", a1.is_sorted());
    //println!("a1 равен {:#?}", a1);
}
