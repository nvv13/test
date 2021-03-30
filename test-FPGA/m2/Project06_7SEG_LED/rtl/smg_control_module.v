/******************************************/  
//     Модуль управления цифровыми лампами //
/******************************************/  
module smg_control_module
(
    input CLK1MS,
	 input RSTn,
	 input [15:0]Number_Sig,
	 output [3:0]Number_Data
);


	 /******************************************/ 
	 
	 reg [3:0]i;
	 reg [3:0]rNumber;
	 
	 always @ ( posedge CLK1MS or negedge RSTn )
	     if( !RSTn )
		      begin
		          i <= 4'd0;
			    	 rNumber <= 4'd0;
				end
		  else 
		      case( i )
					 0: begin
					     i <= i + 1'b1;
					     rNumber <= Number_Sig[15:12];           //тысячная  , раряд 3
  					    end
					 
					 1: begin
					     i <= i + 1'b1;
					     rNumber <= Number_Sig[11:8];            //сотни , раряд 3
  					    end
					 
					 2: begin
					     i <= 4'd0;
					     rNumber <= Number_Sig[7:4];            //Десятки , раряд 2
  					    end

					 3: begin
					     i <= 4'd0;
					     rNumber <= Number_Sig[3:0];            //еденицы , раряд 1
  					    end
			 
				endcase
				
    /******************************************/ 
	 
	 assign Number_Data = rNumber;
	 
	 /******************************************/
	 
endmodule
