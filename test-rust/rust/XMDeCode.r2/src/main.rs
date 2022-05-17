use std::env;

pub fn hex_byte_to_byte(c_in: u8) -> u8 {
    let b_out: u8 = match c_in {
        48..=57 => c_in - 48,
        65..=70 => c_in - 65 + 10,
        97..=102 => c_in - 97 + 10,
        _ => 0,
    };

    return b_out;
}

pub fn xm_decrypt(s_name: &String) -> String {
    let mut s_pass: String = "".to_string();

    if s_name.len() % 2 == 0 {
        let cnt = s_name.len() / 2;
        let mut number = 0;
        while number != cnt {
            let b_hi: u8 = s_name.as_bytes()[number * 2];
            let b_low: u8 = s_name.as_bytes()[number * 2 + 1];

            let decoded: u8 = hex_byte_to_byte(b_hi) * 16 + hex_byte_to_byte(b_low);

            let out: u8 = (decoded ^ 101) & 0xff;

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
