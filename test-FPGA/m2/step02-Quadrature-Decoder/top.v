// подключал обычный Rotary Encoder, Модуль энкодера, что для Arduino продают x_clk x_dt x_sw
// к bus1 можно светадиоды подключить, видно как число (бинарное) считает , туда- сюда, как вращать...
// идея взята отьсюда https://www.fpga4fun.com/QuadratureDecoder.html

module top(
        (* chip_pin="77", altera_attribute = "-name IO_STANDARD \"3.3-V LVTTL\""  *) output LED,
        (* chip_pin="12" *) input CLK50M,

        (* chip_pin="71", altera_attribute = "-name IO_STANDARD \"3.3V SCHMITT TRIGGER INPUT\"" *) input x_clk,
        (* chip_pin="72", altera_attribute = "-name IO_STANDARD \"3.3V SCHMITT TRIGGER INPUT\"" *) input x_dt,
        (* chip_pin="73", altera_attribute = "-name IO_STANDARD \"3.3V SCHMITT TRIGGER INPUT\"" *) input x_sw,
		  
        (* chip_pin="8,7,6,5,4,3,2,1" *) output [7:0] bus1 
          );


//wire o_wire1;

quad o_quad1(CLK50M,x_clk,x_dt,bus1);
			 
//assign LED = CLK;


endmodule



module quad(clk, quadA, quadB, count);
input clk, quadA, quadB;
output [7:0] count;

reg [2:0] quadA_delayed, quadB_delayed;
always @(posedge clk) quadA_delayed <= {quadA_delayed[1:0], quadA};
always @(posedge clk) quadB_delayed <= {quadB_delayed[1:0], quadB};

wire count_enable = quadA_delayed[1] ^ quadA_delayed[2] ^ quadB_delayed[1] ^ quadB_delayed[2];
wire count_direction = quadA_delayed[1] ^ quadB_delayed[2];

reg [7:0] count;
always @(posedge clk)
begin
  if(count_enable)
  begin
    if(count_direction) count<=count+1; else count<=count-1;
  end
end

endmodule
