`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////
// Module Name:    sd_test
///////////////////////////////////////////////////////////////////
module sd_test(
					input  clk,     //50Mhz input clock      
					input rst_n,
					
					output SD_clk,	    //25Mhz SD SPI时钟				
					output SD_cs,      //SD SPI片选	
					output SD_datain,  //SD SPI数据输入	
					input  SD_dataout  //SD SPI数据输出	
					
    );

wire SD_datain_i;
wire SD_datain_w;
wire SD_datain_r;
reg SD_datain_o;

wire SD_cs_i;
wire SD_cs_w;
wire SD_cs_r;
reg SD_cs_o;


reg [31:0]read_sec;
reg read_req;

reg [31:0]write_sec;
reg write_req;

wire [7:0]mydata_o/* synthesis keep */;
wire myvalid_o/* synthesis keep */;

wire init_o/* synthesis keep */;             //SD 初始化完成标识
wire write_o;                                //SD blcok写完成标识
wire read_o;                                 //SD blcok读完成标识

reg [3:0] sd_state;

wire [3:0] initial_state;
wire [3:0] write_state;
wire [3:0] read_state;

wire rx_valid;

parameter STATUS_INITIAL=4'd0;
parameter STATUS_WRITE=4'd1;
parameter STATUS_READ=4'd2;
parameter STATUS_IDLE=4'd3;

assign SD_cs=SD_cs_o;
assign SD_datain=SD_datain_o;

/*******************************/
//SD卡初始化,block写,block读	
/*******************************/
always @ ( posedge SD_clk or negedge rst_n )
    if( !rst_n ) begin
			sd_state <= STATUS_INITIAL;
			read_req <= 1'b0;
			read_sec <= 32'd0;
			write_req <= 1'b0;
			write_sec <= 32'd0;			
	 end
	 else 
	     case( sd_state )

	      STATUS_INITIAL:      // 等待sd卡初始化结束
			if( init_o ) begin sd_state <= STATUS_WRITE; write_sec <= 32'd0; write_req<=1'b1; end
			else begin sd_state <= STATUS_INITIAL; end	
		  
	      STATUS_WRITE:        //等待sd卡block写结束
			if( write_o ) begin sd_state <= STATUS_READ; read_sec <= 32'd0; read_req<=1'b1; end
			else begin write_req<=1'b0; sd_state <= STATUS_WRITE; end
	
			STATUS_READ:        //等待sd卡block读结束
			if( read_o ) begin sd_state <= STATUS_IDLE; end
			else begin read_req<=1'b0; sd_state <= STATUS_READ;  end
			
	      STATUS_IDLE:        //空闲状态
			sd_state <= STATUS_IDLE;
			
			default: sd_state <= STATUS_IDLE;
	      endcase

//SD卡初始化程序				
sd_initial	sd_initial_inst(					
						.rst_n(rst_n),
						.SD_clk(SD_clk),
						.SD_cs(SD_cs_i),
						.SD_datain(SD_datain_i),
						.SD_dataout(SD_dataout),
						.rx(),
						.init_o(init_o),
						.state(initial_state)

);


//SD卡block读程序, 写512个0~255,0~255的数据			 
sd_write	sd_write_inst(   
						.SD_clk(SD_clk),
						.SD_cs(SD_cs_w),
						.SD_datain(SD_datain_w),
						.SD_dataout(SD_dataout),
						
						.init(init_o),
						.sec(write_sec),
						.write_req(write_req),
						.mystate(write_state),
						.rx_valid(rx_valid),

						.write_o(write_o)			
						
    );

//SD卡block读程序, 读512个数据			 
sd_read	sd_read_inst(   
						.SD_clk(SD_clk),
						.SD_cs(SD_cs_r),
						.SD_datain(SD_datain_r),
						.SD_dataout(SD_dataout),
						
						.init(init_o),
						.sec(read_sec),
						.read_req(read_req),
						
						.mydata_o(mydata_o),
						.myvalid_o(myvalid_o),
		
						.data_come(data_come),
						.mystate(read_state),
						
						.read_o(read_o)
						
    );

//SD卡SPI信号的选择
always @(*)
begin
	 case( sd_state )
	 STATUS_INITIAL: begin SD_cs_o<=SD_cs_i;SD_datain_o<=SD_datain_i; end
	 STATUS_WRITE: begin SD_cs_o<=SD_cs_w;SD_datain_o<=SD_datain_w; end
	 STATUS_READ: begin SD_cs_o<=SD_cs_r;SD_datain_o<=SD_datain_r; end
	 default: begin SD_cs_o<=1'b1;SD_datain_o<=1'b1; end	 
	 endcase
end

//PLL产生25Mhz的SD卡SPI时钟
pll pll_inst(
	.areset(~rst_n),
	.inclk0(clk),
	.c0(SD_clk),
	.locked()
	
		);
	 

endmodule
