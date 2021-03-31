module tst1 ( // Assigns pin location to ports on a module.

     (* chip_pin = "77" , altera_attribute = "-name IO_STANDARD \"3.3-V LVTTL\"" *) output LED0    , // на плате есть 1 светодиод ,на 77 контакте
	  
(* chip_pin = "1" *)   output P_OUT_1
);


reg a1 ;
initial a1 <= 1'b0;

always @ (*)
 begin
   a1 <= ~a1;
 end
 
 
reg led;
initial led <= 1'b0;
 
reg [24:0] counter;
initial counter <= 25'd0;

always @(posedge a1) begin
	if(counter==25'd25000000) begin 
		counter <= 25'd0;
		led <= ~led;
	end else begin
		counter <= counter + 1'b1;
	end
end
 
 
assign P_OUT_1 = a1;
assign LED0 = led;


endmodule

