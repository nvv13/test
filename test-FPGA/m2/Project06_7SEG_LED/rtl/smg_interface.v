/******************************************/
//      Модуль интерфейса цифровой трубки  //
/******************************************/
		
module smg_interface
(
    input CLK,
	 input RSTn,
	 input [15:0]Number_Sig,
	 output [6:0]SMG_Data,
	 output [3:0]Scan_Sig
);

    /******************************************/
	 
	 wire [3:0]Number_Data;
	 
    /******************************************/    
	 
	 parameter T1MS = 16'd49999;            //1ms считать
	 
	 /******************************************/  
	 
	  reg [15:0]C1;
	  wire clk_1ms = (C1 == T1MS);
	 
	 always @ ( posedge CLK or negedge RSTn )
	     if( !RSTn )
		      C1 <= 16'd0;
		  else if( C1 == T1MS )
		      C1 <= 16'd0;
		  else
		      C1 <= C1 + 1'b1;
	 
	 /******************************************/ 	 
	 
	 smg_control_module U1
	 (
	    .CLK1MS( clk_1ms ),
		 .RSTn( RSTn ),
		 .Number_Sig( Number_Sig ),    // input - from top
		 .Number_Data( Number_Data )   // output - to U2
	 );
	 
	 /******************************************/
	 
    smg_encode_module U2
	 (
	     .CLK( clk_1ms ),
		  .RSTn( RSTn ),
		  .Number_Data( Number_Data ),   // input - from U2
		  .SMG_Data( SMG_Data )          // output - to top
	 );
	 
	 /*******************************************/
	 
	 smg_scan_module U3
	 (
	     .CLK1MS( clk_1ms ),
		  .RSTn( RSTn ),
		  .Scan_Sig( Scan_Sig )  // output - to top
	 );
	 
	 /*******************************************/
	 
	 

endmodule
