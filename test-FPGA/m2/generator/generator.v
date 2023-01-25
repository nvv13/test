module generator  // главный модуль, имя совпадает с названием файла
#(parameter WIDTH=26) // это, типа #define , только все внутри модуля
   (  (* chip_pin = "12" *) input clk,  // входной пин , 50 мгц, на 12 контакте
      (* chip_pin = "77" *) output LED, // на плате есть 1 светодиод ,на 77 контакте
      (* chip_pin = "27,21,20,18,16,8,6,4" *) output [7:0] b1, // типа шины , вывел на 8-1 ножки
      (* chip_pin = "2" *) output m1 // 
    );

wire [WIDTH-1:0] a1; // определили "провод", для связи

binary_up_down_counter o_cnt1(clk,1'b1,1'b1,1'b0,a1); // перемннная - типа binary_up_down_counter, это внешний модуль, ниже его текст есть
	 
// соединения	 
assign 	 LED=a1[WIDTH-1]; // светодиоду - старший разряд
assign 	 b1=a1[7:0]; // на шину - младшие разряды
assign 	 m1=clk; // клок

endmodule

// Quartus Prime Verilog Template
// Binary up/down counter
module binary_up_down_counter // этот модуль, взял из шаблонов
#(parameter WIDTH=26)
(
	input clk, enable, count_up, reset,
	output reg [WIDTH-1:0] count
);

	// Reset if needed, increment or decrement if counting is enabled
	always @ (posedge clk or posedge reset)
	begin
		if (reset)
			count <= 0;
		else if (enable == 1'b1)
			count <= count + (count_up ? 1 : -1);
	end

endmodule
