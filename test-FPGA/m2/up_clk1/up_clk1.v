module up_clk1(

      (* chip_pin = "12" *) input clk,  // входной пин , 50 мгц, на 12 контакте
      (* chip_pin = "77" *) output LED // на плате есть 1 светодиод ,на 77 контакте
		);

	
reg a1;
initial a1 <= 1'b0;

reg a2;
initial a2 <= 1'b1;

wire b1;
	
always @(posedge clk) begin
	a1 <= ~a1;
end

always @(negedge clk) begin
	a2 <= ~a2;
end

assign b1 = a1 ^ a2;


reg lk0;
initial lk0 <= 1'b0;

reg [24:0] counter;
initial counter <= 25'd0;

always @(posedge b1) begin
	if(counter==25'd25000000) begin 
		counter <= 25'd0;
		lk0 <= ~lk0;
	end else begin
		counter <= counter + 1'b1;
	end
end

assign LED = lk0;


endmodule
 