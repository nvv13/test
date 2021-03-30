module data_num(clk,txen,mynum);
input clk,txen;
output reg [12:0] mynum;

always @(posedge clk)
 begin
     if(txen)mynum<=mynum+1'b1;
     else mynum<=13'b0;
 end
 
 endmodule
 