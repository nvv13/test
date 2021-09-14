/*
смысл программы
подсоедениться к mssql
и выгрузить табличку с фиас кодами, в кодировке win1251
в файл
*/

use encoding_rs::*;
use sqlx;
use sqlx::mssql::MssqlConnectOptions;
use std::fs::File;
use std::io::prelude::*;
use std::io::LineWriter;

#[derive(sqlx::FromRow)]
struct Data {
    s_ulis_shortname: String,
    s_ulis_name: String,
    houseguid: String,
    housenum: String,
    s_korp: String,
}

//#[async_std::main]
#[tokio::main]
async fn main() -> Result<(), sqlx::Error> {
    let opt = MssqlConnectOptions::new()
        .host("depo5")
        .port(1417)
        .username("user_test_rust_123")
        .password("ваш пароль")
        .database("gis_gkx1_cit1");

    let pool = sqlx::MssqlPool::connect_with(opt).await?;
    let query = "select s_ulis_shortname,s_ulis_name,houseguid
                     ,housenum=isnull(housenum,'')
                     ,s_korp=isnull(s_korp,'')
                from [fias_test].[dbo].[HOUSE_ARX_CITY_20171217];";

    //
    let rows = sqlx::query_as::<_, Data>(query).fetch_all(&pool).await?;

    let file = File::create("text1.txt")?;
    let mut file = LineWriter::new(file);

    for row in &rows {
        let mut s_out = "".to_string();
        if row.s_korp != "" {
            s_out = s_out
                + &row.houseguid
                + &" | ".to_string()
                + &row.s_ulis_shortname
                + &". ".to_string()
                + &row.s_ulis_name
                + &", д.".to_string()
                + &row.housenum.to_string()
                + &", корп.".to_string()
                + &row.s_korp.to_string();
        } else {
            s_out = s_out
                + &row.houseguid
                + &" | ".to_string()
                + &row.s_ulis_shortname
                + &". ".to_string()
                + &row.s_ulis_name
                + &", д.".to_string()
                + &row.housenum.to_string();
        }
        s_out = s_out + "\r\n";

        let mut intermediate_buffer_bytes = [0u8; 4096];
        let mut i = 0;
        for item in s_out.as_bytes().iter() {
            intermediate_buffer_bytes[i] = *item;
            i += 1;
        }
        let intermediate_buffer: &mut str =
            unsafe { std::mem::transmute(&mut intermediate_buffer_bytes[..]) };

        let decoder_written = s_out.len();
        let mut encoder = WINDOWS_1251.new_encoder();
        let mut encoder_input_start = 0usize;
        let mut output_buffer = [0u8; 4096];
        let last_output = true;
        loop {
            let (encoder_result, encoder_read, encoder_written, _) = encoder.encode_from_utf8(
                &intermediate_buffer[encoder_input_start..decoder_written],
                &mut output_buffer,
                last_output,
            );
            encoder_input_start += encoder_read;
            match file.write_all(&output_buffer[..encoder_written]) {
                Err(_) => {
                    print!("Error writing output.");
                    std::process::exit(-6);
                }
                Ok(_) => {}
            }
            match encoder_result {
                CoderResult::InputEmpty => {
                    break;
                }
                CoderResult::OutputFull => {
                    continue;
                }
            }
        }
    }

    file.flush()?;

    Ok(())
}
