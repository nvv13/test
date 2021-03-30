
module hdmi_leds(
			input wire CLK100MHZ,
			input wire KEY0,
			input wire KEY1,
			output wire [7:0]LED,
			output wire [7:0]TMDS,
			//unused inputs
			input wire FTDI_BD0,
			input wire FTDI_BD1,
			input wire FTDI_BD2,
			input wire FTDI_BD3,
			input wire [7:0]FTD,
			input wire [7:0]FTC,
			input wire [19:0]IO
			);
			
wire w_clk_video;
wire w_clk_hdmi;
wire w_locked;

mypll mypll_inst(
			.inclk0( CLK100MHZ ),
			.c0( w_clk_video ), //74MHz
			.c1( w_clk_hdmi ),	//370MHz
			.locked( w_locked )
			);

reg [47:0]counter;
always @( posedge w_clk_video or negedge KEY0 )
begin
	if( ~KEY0 )
		counter <= 0;
	else
	if( KEY1 )
		counter <= counter+1;
end

display display_inst(
		.reset( ~w_locked ),
		.clk_video( w_clk_video ),
		.clk_hdmi( w_clk_hdmi ),
		.red_leds( counter[31:16] ),
		.green_leds( counter[30:15] ),
		.segments( counter[47:16] ),
		.TMDS( TMDS )
	   );

endmodule
