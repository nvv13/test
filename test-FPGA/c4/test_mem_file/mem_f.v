module mem_f(
	 (* chip_pin="T2" *) input clock_50Mhz ,
    (* chip_pin="W13" *) input reset, // reset
	 (* chip_pin="E4" *) output led            // выходной светодиодный индикатор
	 );

	 
reg [7:0] memry [0:31]; // 32 byte memory.
//wire [7:0] memwrd;
wire x;

reg [5:0] counter_mem;// = 0;
reg [3:0] counter_bit;// = 0;
reg x_bit;

reg [31:0] timer;            

reg [5:0] j;// = 0;      

always @(negedge reset)    //
    begin
      if (~reset)                           //
		 begin
		 for (j = 0; j <= 31; j = j + 1)
		   begin
		    memry[j]<= j;
			end 
		 end
    end
		
always @(posedge clock_50Mhz or negedge reset)    //
    begin
      if (~reset)                           //
          timer <= 0;                       //
      else if (timer == 32'd99_999_999)     //
          timer <= 0;                       //
      else
		    timer <= timer + 1'b1;            //
    end

always @( posedge clock_50Mhz)
begin
 if (timer == 32'd99_999_999)  
  begin
   x_bit <= memry[counter_mem][counter_bit];
   counter_mem <= counter_mem + 1'b1;
   counter_bit <= counter_bit + 1'b1;
   if(counter_bit == 3'd8)
     counter_bit<= 3'b000;
   if(counter_mem == 5'd32)
     counter_mem<= 5'b00000;
  // memry[0]<= 8'b01010101;
 end
end


/*
initial begin

 memry[0]<= 8'b01010101;


// Initialize memory contents from file.
$readmemb("init.dat", memry, 8);
// words 8 and 9 are not in the file and will default to x.
end

// Extract last word in memory.
assign memwrd= memry[31];
// Extract most sig bit in word 31
//assign x= memwrd[7];
*/

assign led = x_bit;

endmodule

