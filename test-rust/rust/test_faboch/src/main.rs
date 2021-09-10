fn main() {

/*
Задача: посчитать N-е число последовательности,
 в которой каждый элемент равен сумме двух предыдущих. 
Такая последовательность называется последовательностью Фибоначчи:
 1, 1, 2, 3, 5, 8…

*/

let mut n_e = 100;

    println!("n_e={}",n_e);

let mut var1:u128 = 0;
let mut var2:u128 = 1;
let mut var_tmp:u128;
print!("{}, {}, ", var1, var2);

    while n_e != 0 {
        var_tmp=var1+var2;
        var1=var2;
        var2=var_tmp; 
        print!("{}, ", var2);
        n_e-=1;
    }


}
