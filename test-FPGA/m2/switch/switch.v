module switch(
      (* chip_pin = "77" *) output LED    , // на плате есть 1 светодиод ,на 77 контакте
      (* chip_pin = "12" *) input  clk50m,  // входной пин , 50 мгц, на 12 контакте

		(* chip_pin = "71" *) input  x_clk,   // подключил Энкодер, это вывод  крутилки clk
		(* chip_pin = "72" *) input  x_dt,    //                    это вывод крутилки dt
		(* chip_pin = "73" *) input  x_sw     // это вывод кнопки энкодера sw, в нажатом виде 0

   );

reg bit_a1=1'b0; 
reg bit_a2=1'b0; 
reg bit_a3=1'b0; 
	

	
always @ (negedge x_sw) // по спаду, смотрим 0
 begin
 if(x_sw==1'b0)	
  begin
   bit_a1=~bit_a1;
  end
 end

always @ (negedge x_clk or negedge x_dt) // по спаду, смотрим 0
 begin
 if(x_clk==1'b0)	
  begin
   bit_a2=1'b1;
  end
 if(x_dt==1'b0)	
  begin
   bit_a2=1'b0;
  end
 end





 
assign LED=bit_a2;
	


endmodule



