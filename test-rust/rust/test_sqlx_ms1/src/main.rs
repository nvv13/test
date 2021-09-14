use sqlx;
use sqlx::mssql::MssqlConnectOptions;


#[derive(sqlx::FromRow)]
struct  Data{
    houseguid:String,
    s_ulis_name:String,
}

//#[async_std::main]
#[tokio::main]
async fn main() -> Result<(), sqlx::Error> {

    let opt = MssqlConnectOptions::new()
        .host("depo5")
        .port(1417)
        .username("user_test_rust_123")
        .password("ваш пароль")
        .database("fias_test");

    let pool = sqlx::MssqlPool::connect_with(opt).await?;
    let query="select top 100 houseguid,s_ulis_name from HOUSE_ARX_CITY_20171217;";

    // 
    let rows = sqlx::query_as::<_, Data>(query) 
        .fetch_all(&pool)
        .await?;

    for row in &rows {
        println!("houseguid:{}-s_ulis_name:{}", row.houseguid, row.s_ulis_name);
    }
    Ok(())


    // Make a simple query to return the given parameter
    //let row: (String,) = sqlx::query_as("SELECT @@version")
    //    .fetch_one(&pool).await?;
    //println!("{}", row.0);
    //sqlx::query("D-ELETE FROM articles").execute(&pool).await?;

}
