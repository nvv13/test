use gis_gkx1_cit1
go

sp_help t_k2g_s_orgPPA

select kvitok, s_name=cast(ShortName as nvarchar)  from t_k2g_s_orgPPA
grant select on t_k2g_s_orgPPA to user_test_rust_123