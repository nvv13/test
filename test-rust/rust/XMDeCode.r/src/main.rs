use std::env;

pub fn xm_decrypt(s_name: &String) -> String {
    let mut s_pass: String = "".to_string();

    if s_name.len() % 2 == 0 {
        let cnt = s_name.len() / 2;
        let mut number = 0;
        while number != cnt {
            let slice = &s_name[number * 2..number * 2 + 2];

            let mut decoded = [0; 1];
            hex::decode_to_slice(slice, &mut decoded).expect("Decoding failed");

            let out = (decoded[0] ^ 101) & 0xff;

            s_pass.push(out as char);

            number = number + 1;
        }
    }

    return s_pass;
}

fn main() {
    if env::args().len() != 2 {
        println!("usage DeCrypt : xmdecrypt chipher");
    } else {
        let args: Vec<String> = env::args().collect();

        let passwrd = xm_decrypt(&args[1]);

        println!("{}", passwrd);
    }
}
