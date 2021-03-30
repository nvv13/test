/*****************************/
//     Модуль цифрового сканирования трубок         //
/*****************************/
module smg_scan_module
(
    input CLK1MS, 
	 input RSTn, 
	 output [3:0]Scan_Sig
);
	
	 /*******************************/
	 
	 reg [3:0]i;
	 reg [3:0]rScan;
	 
	 always @ ( posedge CLK1MS or negedge RSTn )
	     if( !RSTn )
		      begin
		          i <= 4'd0;
		          rScan <= 4'b0000;
			   end
		  else 
		      case( i )
				    
					 0: begin
		              i <= i + 1'b1;
					     rScan <= 4'b1000;                      //Первый цифровой стробоскоп
						 end 
					 
					 1: begin 
					     i <= i + 1'b1;
					     rScan <= 4'b0100;                      //второй
					    end
					 
					 2: begin 
					     i <= i + 1'b1;
					     rScan <= 4'b0010;                      //третий
						 end
					 
					 3: begin
					     i <= 4'd0;
					     rScan <= 4'b0001;                      //четвертый
						 end 
				 
				endcase
				
	 /******************************/
	 
	 assign Scan_Sig = rScan;
	 
	 /******************************/
		      

endmodule
