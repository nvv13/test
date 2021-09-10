use lettre::{Message, SmtpTransport, Transport};

use lettre::message::{header, SinglePart};

fn main() {
    let email = Message::builder()
        .from("test <dulis@arhirc.ru>".parse().unwrap())
        .reply_to("Vasia <nvv13@mail.ru>".parse().unwrap())
        .to("NVV <nvv13@arhirc.ru>".parse().unwrap())
        .subject("новая тема")
        .singlepart(
            SinglePart::builder()
                .header(header::ContentType::TEXT_PLAIN) // Content-Type: text/html; charset=utf-8
                .body(String::from("Привет!")),
        ) //
        .unwrap();

    // Send the email
    match SmtpTransport::builder_dangerous("mail.arhirc.ru")
        .port(25)
        .build()
        .send(&email)
    {
        Ok(_) => println!("Email sent successfully!"),
        Err(e) => panic!("Could not send email: {:?}", e),
    }
}
