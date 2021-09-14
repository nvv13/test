use gis_gkx1_cit1
go

sp_help t_k2g_s_orgPPA

select kvitok, s_name=cast(ShortName as nvarchar)  from t_k2g_s_orgPPA
grant select on t_k2g_s_orgPPA to user_test_rust_123

select * from [dbo].[t_k2g_s_House]

select ulisguid,s_ulis_shortname,s_ulis_name,HOUSEGUID,HOUSENUM,s_KORP,STRUCNUM
 from [fias_test].[dbo].[HOUSE_ARX_CITY_20171217]

fias_test..sp_help [HOUSE_ARX_CITY_20171217]
fias_test..sp_helprotect [HOUSE_ARX_CITY_20171217]