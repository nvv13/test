use sqlx;
use sqlx::mssql::MssqlConnectOptions;


#[derive(sqlx::FromRow)]
struct  Data{
    kvitok:i16,
    s_name:String,
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
    let query="select kvitok, s_name=cast(ShortName as nvarchar)  from t_k2g_s_orgPPA;";

    // 
    let rows = sqlx::query_as::<_, Data>(query) 
        .fetch_all(&pool)
        .await?;

    for row in &rows {
        println!("kvitok:{}-s_name:{}", row.kvitok, row.s_name);
    }
    Ok(())


    // Make a simple query to return the given parameter
    //let row: (String,) = sqlx::query_as("SELECT @@version")
    //    .fetch_one(&pool).await?;
    //println!("{}", row.0);
    //sqlx::query("D-ELETE FROM articles").execute(&pool).await?;

}
