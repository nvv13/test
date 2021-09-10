use lettre::message::{Attachment, Body, MultiPart, SinglePart};
use lettre::transport::smtp::authentication::Credentials;
use lettre::{Message, SmtpTransport, Transport};
use std::fs;

fn main() {
    let image = fs::read("разрешение для программы.jpg").unwrap();
    let image_body = Body::new(image);

    let email = Message::builder()
        .from("test <nvv13@mail.ru>".parse().unwrap())
        .reply_to("Vasia <nvv13@mail.ru>".parse().unwrap())
        .to("NVV <nvv13@arhirc.ru>".parse().unwrap())
        .subject("новая тема 2, тест сообщения...")
        .multipart(
            MultiPart::mixed()
                .multipart(
                    MultiPart::alternative()
                        .singlepart(SinglePart::plain(String::from("Тест! :)\nТест! :)")))
                        .multipart(
                            MultiPart::related()
                                .singlepart(SinglePart::html(String::from(
                                    "<p><b>Привет</b>, <i>мир</i>! <img src=cid:123></p>",
                                )))
                                .singlepart(
                                    Attachment::new_inline(String::from("123"))
                                        .body(image_body, "image/png".parse().unwrap()),
                                ),
                        ),
                )
                .singlepart(Attachment::new(String::from("example.rs")).body(
                    String::from("fn main() { println!(\"Это пример!\") }"),
                    "text/plain".parse().unwrap(),
                )),
        )
        .unwrap();

    let creds = Credentials::new(
        "nvv13".to_string(),
        "*******secret**pass***********".to_string(),
    );

    // Open a remote connection to gmail
    let mailer = SmtpTransport::relay("smtp.mail.ru")
        .unwrap()
        .credentials(creds)
        .build();

    // Send the email
    match mailer.send(&email) {
        Ok(_) => println!("Email sent successfully!"),
        Err(e) => panic!("Could not send email: {:?}", e),
    }
}
