//marsohod3 board has only 8 yellow LEDs
//but sample MIPS needs red/green LEDs and 7-segment indicators
//so try display LEDs and 7-segment info on HDMI output of Marsohod3 board

module display 
		(
		input wire reset,
		input wire clk_video,	//74MHz
		input wire clk_hdmi,		//370MHz
		input wire [NUM_RED_LEDS-1:0]red_leds,
		input wire [NUM_GREEN_LEDS-1:0]green_leds,
		input wire [NUM_SEGMENTS*4-1:0]segments,
		//HDMI output
		output wire [7:0]TMDS
	   );
	   
parameter NUM_RED_LEDS = 16;
parameter NUM_GREEN_LEDS = 16;
parameter NUM_SEGMENTS = 6;

wire w_hsync;
wire w_vsync;
wire w_active;
wire [11:0]w_pixel_count;
wire [11:0]w_line_count;

wire w_video_clk5; assign w_video_clk5 = clk_hdmi;
wire w_video_clk; assign w_video_clk = clk_video;

hvsync u_hvsync(
	.reset( reset ),
	.pixel_clock( w_video_clk ),

	.hsync(w_hsync),
	.vsync(w_vsync),
	.active(w_active),

	.pixel_count(w_pixel_count),
	.line_count(w_line_count),
	.dbg( )
	);

reg d_active;
reg r_hsync;
reg r_vsync;
reg r_vsync_;
reg [NUM_RED_LEDS-1:0]red_leds_fixed;
reg [NUM_GREEN_LEDS-1:0]green_leds_fixed;
reg [NUM_SEGMENTS*4-1:0]segments_fixed;

always @(posedge w_video_clk)
begin
	r_hsync  <= w_hsync;
	r_vsync  <= w_vsync;
	r_vsync_ <= r_vsync;
	d_active <= w_active;
	if( r_vsync_==1'b0 && r_vsync==1'b1 )
	begin
		red_leds_fixed <= red_leds;
		green_leds_fixed <= green_leds;
		segments_fixed <= segments;
	end
end

reg [9:0]red_word;
reg [9:0]green_word;
reg [9:0]blue_word;

wire led_visible; assign led_visible = w_pixel_count<512 && w_pixel_count[4]==1'b1;
wire [3:0]led_idx; assign led_idx = NUM_RED_LEDS-1-w_pixel_count[8:5];
wire current_green_led = (green_leds_fixed >> led_idx)&1;
wire current_red_led = (red_leds_fixed >> led_idx)&1;

wire h_seg_line5; assign h_seg_line5 = w_pixel_count<(64*1-8) && w_pixel_count>(64*0+32);
wire h_seg_line4; assign h_seg_line4 = w_pixel_count<(64*2-8) && w_pixel_count>(64*1+32);
wire h_seg_line3; assign h_seg_line3 = w_pixel_count<(64*3-8) && w_pixel_count>(64*2+32);
wire h_seg_line2; assign h_seg_line2 = w_pixel_count<(64*4-8) && w_pixel_count>(64*3+32);
wire h_seg_line1; assign h_seg_line1 = w_pixel_count<(64*5-8) && w_pixel_count>(64*4+32);
wire h_seg_line0; assign h_seg_line0 = w_pixel_count<(64*6-8) && w_pixel_count>(64*5+32);

wire vl_seg_line5; assign vl_seg_line5 = w_pixel_count<(64*1-32) && w_pixel_count>(64*0+24);
wire vl_seg_line4; assign vl_seg_line4 = w_pixel_count<(64*2-32) && w_pixel_count>(64*1+24);
wire vl_seg_line3; assign vl_seg_line3 = w_pixel_count<(64*3-32) && w_pixel_count>(64*2+24);
wire vl_seg_line2; assign vl_seg_line2 = w_pixel_count<(64*4-32) && w_pixel_count>(64*3+24);
wire vl_seg_line1; assign vl_seg_line1 = w_pixel_count<(64*5-32) && w_pixel_count>(64*4+24);
wire vl_seg_line0; assign vl_seg_line0 = w_pixel_count<(64*6-32) && w_pixel_count>(64*5+24);

wire vr_seg_line5; assign vr_seg_line5 = w_pixel_count<(64*1) && w_pixel_count>(64*0+56);
wire vr_seg_line4; assign vr_seg_line4 = w_pixel_count<(64*2) && w_pixel_count>(64*1+56);
wire vr_seg_line3; assign vr_seg_line3 = w_pixel_count<(64*3) && w_pixel_count>(64*2+56);
wire vr_seg_line2; assign vr_seg_line2 = w_pixel_count<(64*4) && w_pixel_count>(64*3+56);
wire vr_seg_line1; assign vr_seg_line1 = w_pixel_count<(64*5) && w_pixel_count>(64*4+56);
wire vr_seg_line0; assign vr_seg_line0 = w_pixel_count<(64*6) && w_pixel_count>(64*5+56);

function  [7:0]seg7;
input [3:0]a;
begin
	case(a)
	0: seg7 = 63;
	1: seg7 = 6;
	2: seg7 = 91;
	3: seg7 = 79;
	4: seg7 = 102;
	5: seg7 = 109;
	6: seg7 = 125;
	7: seg7 = 7;
	8: seg7 = 127;
	9: seg7 = 111;
	10: seg7 = 119;
	11: seg7 = 124;
	12: seg7 = 57;
	13: seg7 = 94;
	14: seg7 = 121;
	15: seg7 = 113;
	endcase
end
endfunction

reg [6:0]seg7_0;
reg [6:0]seg7_1;
reg [6:0]seg7_2;
reg [6:0]seg7_3;
reg [6:0]seg7_4;
reg [6:0]seg7_5;

always @(posedge clk_video)
begin
	seg7_0 <= seg7( segments_fixed[ 3: 0] );
	seg7_1 <= seg7( segments_fixed[ 7: 4] );
	seg7_2 <= seg7( segments_fixed[11: 8] );
	seg7_3 <= seg7( segments_fixed[15:12] );
	seg7_4 <= seg7( segments_fixed[19:16] );
	seg7_5 <= seg7( segments_fixed[23:20] );
end

reg [2:0]color;

always @*
begin
	if( w_line_count>64 && w_line_count<80 ) begin
		//green led line
		if(led_visible)
			color = current_green_led ? 1 : 2;
		else color=0;
	end
	else
	if( w_line_count>96 && w_line_count<112 ) begin
		//red led line
		if(led_visible)
			color = current_red_led ? 3 : 4;
		else color=0;
	end
	else
	if( w_line_count>118+32 && w_line_count<126+32 ) begin
		// 7seg top line
		if(h_seg_line0)
			color = seg7_0[0] ? 5 : 6;
		else
		if(h_seg_line1)
			color = seg7_1[0] ? 5 : 6;
		else 
		if(h_seg_line2)
			color = seg7_2[0] ? 5 : 6;
		else 
		if(h_seg_line3)
			color = seg7_3[0] ? 5 : 6;
		else 
		if(h_seg_line4)
			color = seg7_4[0] ? 5 : 6;
		else 
		if(h_seg_line5)
			color = seg7_5[0] ? 5 : 6;
		else 
			color=0;
	end
	else
	if( w_line_count>126+32 && w_line_count<148+32 ) begin
		if(vr_seg_line0)
			color = seg7_0[1] ? 5 : 6;
		else
		if(vr_seg_line1)
			color = seg7_1[1] ? 5 : 6;
		else 
		if(vr_seg_line2)
			color = seg7_2[1] ? 5 : 6;
		else 
		if(vr_seg_line3)
			color = seg7_3[1] ? 5 : 6;
		else 
		if(vr_seg_line4)
			color = seg7_4[1] ? 5 : 6;
		else 
		if(vr_seg_line5)
			color = seg7_5[1] ? 5 : 6;
		else 
		if(vl_seg_line0)
			color = seg7_0[5] ? 5 : 6;
		else
		if(vl_seg_line1)
			color = seg7_1[5] ? 5 : 6;
		else 
		if(vl_seg_line2)
			color = seg7_2[5] ? 5 : 6;
		else 
		if(vl_seg_line3)
			color = seg7_3[5] ? 5 : 6;
		else 
		if(vl_seg_line4)
			color = seg7_4[5] ? 5 : 6;
		else 
		if(vl_seg_line5)
			color = seg7_5[5] ? 5 : 6;
		else
			color=0;
	end
	else
	if( w_line_count>148+32 && w_line_count<156+32 ) begin
		// 7seg middle line
		if(h_seg_line0)
			color = seg7_0[6] ? 5 : 6;
		else
		if(h_seg_line1)
			color = seg7_1[6] ? 5 : 6;
		else 
		if(h_seg_line2)
			color = seg7_2[6] ? 5 : 6;
		else 
		if(h_seg_line3)
			color = seg7_3[6] ? 5 : 6;
		else 
		if(h_seg_line4)
			color = seg7_4[6] ? 5 : 6;
		else 
		if(h_seg_line5)
			color = seg7_5[6] ? 5 : 6;
		else 
			color=0;
	end
	else
	if( w_line_count>156+32 && w_line_count<180+32 ) begin
		if(vr_seg_line0)
			color = seg7_0[2] ? 5 : 6;
		else
		if(vr_seg_line1)
			color = seg7_1[2] ? 5 : 6;
		else 
		if(vr_seg_line2)
			color = seg7_2[2] ? 5 : 6;
		else 
		if(vr_seg_line3)
			color = seg7_3[2] ? 5 : 6;
		else 
		if(vr_seg_line4)
			color = seg7_4[2] ? 5 : 6;
		else 
		if(vr_seg_line5)
			color = seg7_5[2] ? 5 : 6;
		else 
		if(vl_seg_line0)
			color = seg7_0[4] ? 5 : 6;
		else
		if(vl_seg_line1)
			color = seg7_1[4] ? 5 : 6;
		else 
		if(vl_seg_line2)
			color = seg7_2[4] ? 5 : 6;
		else 
		if(vl_seg_line3)
			color = seg7_3[4] ? 5 : 6;
		else 
		if(vl_seg_line4)
			color = seg7_4[4] ? 5 : 6;
		else 
		if(vl_seg_line5)
			color = seg7_5[4] ? 5 : 6;
		else
			color=0;
	end
	else
	if( w_line_count>180+32 && w_line_count<188+32 ) begin
		// 7seg bottom line
		if(h_seg_line0)
			color = seg7_0[3] ? 5 : 6;
		else
		if(h_seg_line1)
			color = seg7_1[3] ? 5 : 6;
		else 
		if(h_seg_line2)
			color = seg7_2[3] ? 5 : 6;
		else 
		if(h_seg_line3)
			color = seg7_3[3] ? 5 : 6;
		else 
		if(h_seg_line4)
			color = seg7_4[3] ? 5 : 6;
		else 
		if(h_seg_line5)
			color = seg7_5[3] ? 5 : 6;
		else 
			color=0;
	end
	else
		color=0;
end

reg [2:0]color_fixed;
always @(posedge clk_video)
	color_fixed<=color;

always @*
begin
	case(color_fixed)
	0: begin //gray
			red_word   = 10'b0000011111;
			green_word = 10'b0000011111;
			blue_word  = 10'b0000111111;
		end
	1: begin //bright green
			red_word   = 10'b0000011111;
			green_word = 10'b1111111111;
			blue_word  = 10'b0000011111;
		end
	2: begin //dark green
			red_word   = 10'b0000011111;
			green_word = 10'b0000111111;
			blue_word  = 10'b0000011111;
		end
	3: begin //bright red
			red_word   = 10'b1111111111;
			green_word = 10'b0000011111;
			blue_word  = 10'b0000011111;
		end
	4: begin //dark red
			red_word   = 10'b0000111111;
			green_word = 10'b0000011111;
			blue_word  = 10'b0000011111;
		end
	5: begin //bright yellow
			red_word   = 10'b1111111111;
			green_word = 10'b1111111111;
			blue_word  = 10'b0000011111;
		end
	6: begin //dark yellow
			red_word   = 10'b0000111111;
			green_word = 10'b0000111111;
			blue_word  = 10'b0000011111;
		end
	7: begin //gray
			red_word   = 10'b0000011111;
			green_word = 10'b0000011111;
			blue_word  = 10'b0000111111;
		end		
	endcase
end

wire w_tmds_bh;
wire w_tmds_bl;
wire w_tmds_gh;
wire w_tmds_gl;
wire w_tmds_rh;
wire w_tmds_rl;

hdmi u_hdmi(
	.pixclk( w_video_clk ),
	.clk_TMDS2( w_video_clk5 ),
	.hsync( r_hsync ),
	.vsync( r_vsync ),
	.active( d_active ),
	.red( red_word ),
	.green( green_word ),
	.blue( blue_word ),
	.TMDS_bh( w_tmds_bh ),
	.TMDS_bl( w_tmds_bl ),
	.TMDS_gh( w_tmds_gh ),
	.TMDS_gl( w_tmds_gl ),
	.TMDS_rh( w_tmds_rh ),
	.TMDS_rl( w_tmds_rl )
);

altddio_out1 u_ddio1( .datain_h( w_video_clk), .datain_l( w_video_clk), .outclock(w_video_clk5), .dataout( TMDS[1] ) );
altddio_out1 u_ddio0( .datain_h(~w_video_clk), .datain_l(~w_video_clk), .outclock(w_video_clk5), .dataout( TMDS[0] ) );
altddio_out1 u_ddio3( .datain_h( w_tmds_bh),   .datain_l( w_tmds_bl),   .outclock(w_video_clk5), .dataout( TMDS[3] ) );
altddio_out1 u_ddio2( .datain_h(~w_tmds_bh),   .datain_l(~w_tmds_bl),   .outclock(w_video_clk5), .dataout( TMDS[2] ) );
altddio_out1 u_ddio5( .datain_h( w_tmds_gh),   .datain_l( w_tmds_gl),   .outclock(w_video_clk5), .dataout( TMDS[5] ) );
altddio_out1 u_ddio4( .datain_h(~w_tmds_gh),   .datain_l(~w_tmds_gl),   .outclock(w_video_clk5), .dataout( TMDS[4] ) );
altddio_out1 u_ddio7( .datain_h( w_tmds_rh),   .datain_l( w_tmds_rl),   .outclock(w_video_clk5), .dataout( TMDS[7] ) );
altddio_out1 u_ddio6( .datain_h(~w_tmds_rh),   .datain_l(~w_tmds_rl),   .outclock(w_video_clk5), .dataout( TMDS[6] ) );

endmodule
