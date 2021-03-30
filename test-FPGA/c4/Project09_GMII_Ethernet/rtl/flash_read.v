`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:51:09 02/20/2013 
// Design Name: 
// Module Name:    flash_read 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module flash_read(


						output flash_cs,
						output flash_datain,
						input  flash_dataout,
						
                  input clock,
						input flash_rst,
						input read_req,
						output [7:0]mydata_o,
						output myvalid_o,
                  output flash_read_done						
						);


reg flash_cs;
reg flash_datain; 

assign myvalid_o=myvalid;
assign mydata_o=mydata;

assign flash_clk=spi_clk_en?clock:0;


reg myvalid;
reg [7:0] mydata;
reg flash_read_done=1'b0;

reg [2:0] mystate;
reg spi_clk_en=1'b0;


parameter idle=3'b000;
parameter cmd_send=3'b001;
parameter address_send=3'b010;
parameter read_wait=3'b011;
parameter read_done=3'b100;


parameter FLASH_ADDR=24'ha0600;        //图像在flash存储的地址
parameter FLASH_READ=8'b00000011;      //flash读的命令字节序列


reg [7:0] cmd_reg=FLASH_READ;
reg [23:0] address_reg=FLASH_ADDR;
reg [7:0] cnta;
reg [7:0] cntb;
reg [19:0] cnt;
reg data_come;
reg read_finish;


//发送读flash命令
always @(negedge clock)
begin
if(flash_rst)
	begin
		flash_cs<=1'b1;		
		mystate<=idle;
		cmd_reg<=0;
		address_reg<=0;
	   spi_clk_en<=1'b0;                //SPI clock输出不使能
		cnta<=0;
      address_reg<=FLASH_ADDR;
		flash_read_done<=1'b0;
		data_come<=1'b0;
	end
else
	begin
	case(mystate)
		idle:
			begin			  
				spi_clk_en<=1'b0;
				flash_cs<=1'b1;
				flash_datain<=1'b1;	
			   cmd_reg<=FLASH_READ;
            address_reg<=address_reg+1;
				if(read_req)                //如果读请求
					begin
						mystate<=cmd_send;
                  cnta<=7;							
					end
			end
		cmd_send:
			begin
			   spi_clk_en<=1'b1;                           //flash的SPI clock输出
				flash_cs<=1'b0;                            //cs拉低
			   if(cnta>0)                                 //如果cmd_reg还没有发送完
					begin
						flash_datain<=cmd_reg[cnta];            //发送bit7~bit1位
                  cnta<=cnta-1;						
					end				
				 else                                       //发送bit0
				   begin 
						flash_datain<=cmd_reg[0];              
					   mystate<=address_send;
						cnta<=23;		 
				   end
			 end
		address_send:
			begin
			   if(cnta>0)                                       //如果cmd_reg还没有发送完
					begin
						flash_datain<=address_reg[cnta];               //发送bit23~bit1位
                  cnta<=cnta-1;						
					end				
				 else                                              //发送bit0
				   begin 
						flash_datain<=address_reg[0];              
					   mystate<=read_wait;
						cnta<=0;             						
				   end
			 end
		read_wait:
		  begin
		     if(read_finish)
			    begin
			     mystate<=read_done;
				  data_come<=1'b0;
				 end
			  else
			     data_come<=1'b1;
		  end
		read_done:
			begin
 		     flash_cs<=1'b1;
			  flash_datain<=1'b1;
			  flash_read_done<=1'b1;
			  spi_clk_en<=1'b0;
			end
		default:mystate<=0;
		endcase;		
	end
end	

//接收flash数据	
always @(posedge clock)
begin
if(flash_rst)
	begin
		cnt<=0;
		cntb<=0;
		read_finish<=1'b0;
		myvalid<=1'b0;
		mydata<=0;
	end
else
    if(data_come)                        
		 begin
			if(cnt<391680)   //读391680字节的图像数据:480*272*3
			  begin
		      if(cntb<7)                     //接收一个byte的bit0~bit6
				  begin
				    myvalid<=1'b0;
				    mydata<={mydata[6:0],flash_dataout};
				    cntb<=cntb+1'b1;
				  end
				else
				  begin
				    myvalid<=1'b1;          //一个byte数据有效
					 mydata<={mydata[6:0],flash_dataout};  //接收bit7
					 cntb<=0;
					 cnt<=cnt+1'b1;
				  end
			  end				 			 
			else 
			  begin 
			    cnt<=0;
				 read_finish<=1'b1;
				 myvalid<=1'b0;
			  end
		end
	 else
      begin
		  cnt<=0;
		  cntb<=0;
		  read_finish<=1'b0;
		  myvalid<=1'b0;
		  mydata<=0;
      end
end			

endmodule

