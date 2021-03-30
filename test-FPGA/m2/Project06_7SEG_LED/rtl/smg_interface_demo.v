module smg_interface_demo
(
    (* chip_pin="12" *) input CLK,
	 (* chip_pin="1" *) input RSTn,
	 (* chip_pin="50,51,52,53,54,55,56",  altera_attribute = "-name IO_STANDARD \"3.3-V LVTTL\""  *) output [6:0]SMG_Data,           //（LEDA .. LEDH）
	 
	 (* chip_pin="26,27,28,29", altera_attribute = "-name IO_STANDARD \"3.3-V LVTTL\""  *) output [3:0]Scan_Sig            //（SEL0_T~SEL2_T）
);

    /******************************/ 
 
    wire [15:0]Number_Sig;
	 
    demo_control_module U1
	 (
	     .CLK( CLK ),
		  .RSTn( RSTn ),
		  .Number_Sig( Number_Sig ) // output - to U2
	 );
	 
	 /******************************/ 
	 
	 smg_interface U2
	 (
	     .CLK( CLK ),
		  .RSTn( RSTn ),
		  .Number_Sig( Number_Sig ), // input - from U1
		  .SMG_Data( SMG_Data ),     // output - to top
		  .Scan_Sig( Scan_Sig )      // output - to top
	 );
	 
    /******************************/ 

endmodule
