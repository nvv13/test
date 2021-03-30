`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Module Name:    pll_test 
//////////////////////////////////////////////////////////////////////////////////
module pll_test(
                input clk,
                input rst_n,
					 output led_1
    );

wire locked;

parameter DLY_CNT = 32'd50000000;
parameter HALF_DLY_CNT = 32'd25000000;

(*mark_debug = "true"*)reg r_led;
(*mark_debug = "true"*)reg [31:0]count;

wire clock_50_pll;
wire clkout4;

//counter control
always@(posedge clkout4 or negedge rst_n)
begin
	if(!rst_n)
		begin
			count <= 32'd0;
		end
	else if(count == DLY_CNT)
		begin
			count <= 32'd0;
		end
	else
		begin
			count <= count+32'd1;
		end
end

//led output register control
always@(posedge clkout4 or negedge rst_n)
begin
	if(!rst_n)
		begin
			r_led <= 1'b0;
		end
	else if(count < HALF_DLY_CNT)
		begin
			r_led <= 1'b1;
		end
	else
		begin
			r_led <= 1'b0;
		end
end

assign led_1 = r_led;


/////////////////////PLL IP cоздаем экземпляр ////////////////////////////
pll pll_inst
   (// Clock in ports
    .inclk0(clk),            	// IN 50Mhz
    // Clock out ports
    .c0(clkout1),     			// OUT 25Mhz
    .c1(clkout2),             // OUT 50Mhz
    .c2(clkout3),             // OUT 75Mhz
    .c3(clkout4),             // OUT 100Mhz	 
    // Status and control signals	 
    .areset(~rst_n),				// IN
    .locked(locked));      	// OUT	  

endmodule
