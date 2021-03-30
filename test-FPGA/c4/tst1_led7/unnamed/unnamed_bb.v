
module unnamed (
	clk,
	reset,
	address,
	chipselect,
	byteenable,
	read,
	write,
	writedata,
	readdata,
	waitrequest,
	irq,
	PS2_CLK,
	PS2_DAT);	

	input		clk;
	input		reset;
	input		address;
	input		chipselect;
	input	[3:0]	byteenable;
	input		read;
	input		write;
	input	[31:0]	writedata;
	output	[31:0]	readdata;
	output		waitrequest;
	output		irq;
	inout		PS2_CLK;
	inout		PS2_DAT;
endmodule
