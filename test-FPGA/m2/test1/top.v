module top(
        (* chip_pin="77" *) output LED,
        (* chip_pin="12" *) input CLK 
          );


wire o_wire1;

tic1sec o_tic1sec1(o_wire1,CLK);
tic1sec o_tic1sec2(LED,o_wire1);
			 
//assign LED = CLK;


endmodule


module tic1sec (output clk1sec, input clk50mz);

reg [25:0] counter = 26'b0;

always @(posedge clk50mz)
begin
 counter<=counter+1'b1;
end

assign clk1sec=counter[25];

endmodule
