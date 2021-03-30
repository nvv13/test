module test2_bl (output LED0)

while @(*)
begin
	// Statements
 assign	LED0 <= ~LED0;
end;




endmodule

