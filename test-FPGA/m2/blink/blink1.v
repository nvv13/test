module blink1(clk, led0, led1, led2);
input wire clk;
output wire led0, led1, led2;

reg led;
initial led <= 1'b0;

reg lnd;
initial lnd <= 1'b0;

reg [24:0] counter;
initial counter <= 25'd0;


always @(posedge clk) begin
	if(counter==25'd25000000) begin 
		counter <= 25'd0;
		led <= ~led;
	end else begin
		counter <= counter + 1'b1;
	end
end

always @(negedge clk) begin
	lnd <= ~lnd;
end


assign led0 = ~led;
assign led1 = lnd;
assign led2 = clk;


endmodule
