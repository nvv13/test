use dialog::backends;
use dialog::DialogBox;


fn main() {


//let mut backend = backends::KDialog::new();
//let mut backend = backends::Dialog::new();
//let backend = backends::Stdio::new(); //console based

/*
Чтоб это заработало под windows, скачал 
https://github.com/kvaps/zenity-windows
и поставил
https://github.com/kvaps/zenity-windows/releases/download/v3.20.0-1/zenity-3.20.0_win32-1.exe
*/


let mut backend = backends::Zenity::new();

backend.set_icon("Temp");

let name = dialog::Input::new("Input C temperature")
    .title("C=")
    .show_with(&backend)
    .expect("Could not display dialog box");

match name {
    Some(name) => 
 {
  // println!("name = l{}l",name.replace("\r", ""));
  let four: f64 = name.replace("\r", "").replace(",", ".").trim().parse().unwrap();
    dialog::Message::new(((four*1.8000)+32 as f64).to_string())
     .title("C => F  temperature")
     .show_with(&backend)
     .expect("Could not display dialog box");

 },
    None => println!("Hello stranger!"),
};

/*
dialog::Message::new(Some(name))
    .title("name")
    .show_with(&backend)
    .expect("Could not display dialog box");
  */


}
