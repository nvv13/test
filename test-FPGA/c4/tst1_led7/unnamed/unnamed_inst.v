	unnamed u0 (
		.clk         (<connected-to-clk>),         //                clk.clk
		.reset       (<connected-to-reset>),       //              reset.reset
		.address     (<connected-to-address>),     //   avalon_ps2_slave.address
		.chipselect  (<connected-to-chipselect>),  //                   .chipselect
		.byteenable  (<connected-to-byteenable>),  //                   .byteenable
		.read        (<connected-to-read>),        //                   .read
		.write       (<connected-to-write>),       //                   .write
		.writedata   (<connected-to-writedata>),   //                   .writedata
		.readdata    (<connected-to-readdata>),    //                   .readdata
		.waitrequest (<connected-to-waitrequest>), //                   .waitrequest
		.irq         (<connected-to-irq>),         //          interrupt.irq
		.PS2_CLK     (<connected-to-PS2_CLK>),     // external_interface.CLK
		.PS2_DAT     (<connected-to-PS2_DAT>)      //                   .DAT
	);

