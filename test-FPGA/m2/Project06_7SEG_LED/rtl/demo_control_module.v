 /******************************/
 /******6 Кусочек BCD Программа генерации счетчика ******/
 /******************************/
module demo_control_module
(
    input CLK,
	 input RSTn,
	 output [15:0]Number_Sig
);
   
	 /******************************/
	 
	 parameter T100MS = 23'd4_999_999;          //100ms Считать
	 
	 /******************************/
	 
	 reg [22:0]C1;
	 
	 always @ ( posedge CLK or negedge RSTn )
	     if( !RSTn )
		      C1 <= 23'd0;
		  else if( C1 == T100MS )
		      C1 <= 23'd0;
		  else 
		      C1 <= C1 + 1'b1;
	
	  /*******************************************************/
	 
	 reg [3:0]i;
	 reg [15:0]rNum;
	 reg [15:0]rNumber;
	 
	 always @ ( posedge CLK or negedge RSTn )
	      if( !RSTn )
			    begin
			        i <= 4'd0;
					  rNum <= 15'd0;
					  rNumber <= 15'd0;
				 end
		   else
			    case( i )
				 
				     0: if( C1 == T100MS ) 
					      begin rNum[3:0] <= rNum[3:0] + 1'b1; i <= i + 1'b1; 
					      end // 1 sig
					  
					  1: if( rNum[3:0] > 4'd9 ) 
					      begin rNum[7:4] <= rNum[7:4] + 1'b1; rNum[3:0] <= 4'd0; i <= i + 1'b1; 
							end // 2 sig
					     else i <= i + 1'b1; 
					  
					  2: if( rNum[7:4] > 4'd9 ) 
					      begin rNum[11:8] <= rNum[11:8] + 1'b1; rNum[7:4] <= 4'd0; i <= i + 1'b1; 
						   end// 3 sig
					     else i <= i + 1'b1;
					  
					  3: if( rNum[11:8] > 4'd9 ) 
					      begin rNum[15:12] <= rNum[15:12] + 1'b1; rNum[11:8] <= 4'd0; i <= i + 1'b1; 
						   end// 4 sig
					     else i <= i + 1'b1;

					  4: begin rNumber <= rNum; i <= 4'd0; 
					     end
				 
				 endcase

    /*******************************************************/
	 
	 assign Number_Sig = rNumber;
	 
	 /*******************************************************/
	 
endmodule
