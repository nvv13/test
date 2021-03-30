
///////////////////////////////////////////////////////////////
//module which generates video sync impulses
///////////////////////////////////////////////////////////////

module hvsync (
	input wire reset,
	input wire pixel_clock,

	output reg hsync,
	output reg vsync,
	output reg active,

	output reg [11:0]pixel_count,
	output reg [11:0]line_count,
	output reg dbg
	);
	
// video signal parameters 1280 x 720 at 60 Hz (non-interlaced) 
//Pixel Clock 74,250Mhz
parameter horz_front_porch = 110;
parameter horz_sync = 40;
parameter horz_back_porch = 220;
parameter horz_addr_time = 1280;

parameter vert_front_porch = 5;
parameter vert_sync = 5;
parameter vert_back_porch = 20;
parameter vert_addr_time = 720;

always @(posedge pixel_clock or posedge reset)
	if(reset)
	begin
		hsync <= 1'b0;
		pixel_count <= 0;
	end
	else
	begin
		hsync <= (pixel_count >= (horz_addr_time+horz_front_porch-1)) && (pixel_count < (horz_addr_time+horz_front_porch+horz_sync-1));
		
		if(pixel_count < (horz_addr_time+horz_front_porch+horz_sync+horz_back_porch-1) )
			pixel_count <= pixel_count + 1'b1;
		else
			pixel_count <= 0;
	end

always @(posedge hsync or posedge reset)
	if(reset)
	begin
		vsync <= 1'b0;
		line_count <= vert_addr_time; //0;
	end
	else
	begin
		vsync <= (line_count >= (vert_addr_time+vert_front_porch-1)) &&  (line_count < (vert_addr_time+vert_front_porch+vert_sync-1) );
		
		if(line_count < (vert_addr_time+vert_front_porch+vert_sync+vert_back_porch -1) )
			line_count <= line_count + 1'b1;
		else
			line_count <= 0;
	end

always @*
	active = (pixel_count<horz_addr_time) && (line_count<vert_addr_time);

always @(posedge hsync)
	dbg <= (line_count>500);
	
endmodule

