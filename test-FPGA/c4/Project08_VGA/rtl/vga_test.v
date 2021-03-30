`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Module Name:    vga_test 
//
//////////////////////////////////////////////////////////////////////////////////
module vga_test(
			input clk,
			input rstn,
			output vga_hs,
			output vga_vs,
			output [4:0] vga_r,
			output [5:0] vga_g,
			output [4:0] vga_b,
			input key1                        // кнопка key1
    );
//-----------------------------------------------------------//
// Настройка параметров горизонтальной развертки 1024*768 60Hz VGA
//-----------------------------------------------------------//
parameter LinePeriod =1344;            //Количество циклов линии
parameter H_SyncPulse=136;             //Линейный синхроимпульс（Sync a）
parameter H_BackPorch=160;             //Показать задний край（Back porch b）
parameter H_ActivePix=1024;            //Отобразить временной сегмент（Display interval c）
parameter H_FrontPorch=24;             //Граница показа（Front porch d）
parameter Hde_start=296;
parameter Hde_end=1320;

//-----------------------------------------------------------//
// Настройка параметров вертикальной развертки 1024*768 60Hz VGA
//-----------------------------------------------------------//
parameter FramePeriod =806;           //Количество циклов колонки
parameter V_SyncPulse=6;              //Импульс синхронизации столбца（Sync o）
parameter V_BackPorch=29;             //Показать задний край（Back porch p）
parameter V_ActivePix=768;            //Отобразить временной сегмент（Display interval q）
parameter V_FrontPorch=3;             //Граница показа（Front porch r）
parameter Vde_start=35;
parameter Vde_end=803;

//-----------------------------------------------------------//
// Настройка параметров горизонтальной развертки 800*600 VGA
//-----------------------------------------------------------//
//parameter LinePeriod =1056;           //Количество циклов линии
//parameter H_SyncPulse=128;            //Линейный синхроимпульс（Sync a）
//parameter H_BackPorch=88;             //Показать задний край（Back porch b）
//parameter H_ActivePix=800;            //Отобразить временной сегмент（Display interval c）
//parameter H_FrontPorch=40;            //Граница показа（Front porch d）

//-----------------------------------------------------------//
// Настройка параметров вертикальной развертки 800*600 VGA
//-----------------------------------------------------------//
//parameter FramePeriod =628;           //Количество циклов колонки
//parameter V_SyncPulse=4;              //Импульс синхронизации столбца（Sync o）
//parameter V_BackPorch=23;             //Показать задний край（Back porch p）
//parameter V_ActivePix=600;            //Отобразить временной сегмент（Display interval q）
//parameter V_FrontPorch=1;             //Граница показа（Front porch r）


  reg[10 : 0] x_cnt;
  reg[9 : 0]  y_cnt;
  reg[15 : 0] grid_data_1;
  reg[15 : 0] grid_data_2;
  reg[15 : 0] bar_data;
  reg[3 : 0] vga_dis_mode;
  reg[4 : 0]  vga_r_reg;
  reg[5 : 0]  vga_g_reg;
  reg[4 : 0]  vga_b_reg;  
  reg hsync_r;
  reg vsync_r; 
  reg hsync_de;
  reg vsync_de;
  
  reg [19:0] key1_counter;                 //Регистр обнаружения ключей
  
  wire vga_clk; 
  wire [12:0]  bar_interval;
  
assign	bar_interval 	= H_ActivePix[15: 3];         //Ширина цветовой полосы=H_ActivePix/8
  
//----------------------------------------------------------------
////////// Количество горизонтальных сканирований
//----------------------------------------------------------------
always @ (posedge vga_clk)
       if(~rstn)    x_cnt <= 1;
       else if(x_cnt == LinePeriod) x_cnt <= 1;
       else x_cnt <= x_cnt+ 1;
		 
//----------------------------------------------------------------
////////// Сигнал горизонтальной развертки hsync,hsync_de производить
//----------------------------------------------------------------
always @ (posedge vga_clk)
   begin
       if(~rstn) hsync_r <= 1'b1;
       else if(x_cnt == 1) hsync_r <= 1'b0;            //производить hsync сигнал
       else if(x_cnt == H_SyncPulse) hsync_r <= 1'b1;
		 
		 		 
	    if(1'b0) hsync_de <= 1'b0;
       else if(x_cnt == Hde_start) hsync_de <= 1'b1;    //производить hsync_de сигнал
       else if(x_cnt == Hde_end) hsync_de <= 1'b0;	
	end

//----------------------------------------------------------------
////////// Количество вертикальных сканирований
//----------------------------------------------------------------
always @ (posedge vga_clk)
       if(~rstn) y_cnt <= 1;
       else if(y_cnt == FramePeriod) y_cnt <= 1;
       else if(x_cnt == LinePeriod) y_cnt <= y_cnt+1;

//----------------------------------------------------------------
////////// Сигнал вертикальной развертки vsync, vsync_de производить
//----------------------------------------------------------------
always @ (posedge vga_clk)
  begin
       if(~rstn) vsync_r <= 1'b1;
       else if(y_cnt == 1) vsync_r <= 1'b0;    // производить vsync сигнал
       else if(y_cnt == V_SyncPulse) vsync_r <= 1'b1;
		 
	    if(~rstn) vsync_de <= 1'b0;
       else if(y_cnt == Vde_start) vsync_de <= 1'b1;    // производить vsync_de сигнал
       else if(y_cnt == Vde_end) vsync_de <= 1'b0;	 
  end
		 

//----------------------------------------------------------------
////////// Генерация тестового изображения сетки
//----------------------------------------------------------------
 always @(negedge vga_clk)   
   begin
     if ((x_cnt[4]==1'b1) ^ (y_cnt[4]==1'b1))            //Создать маленькое сеточное изображение
			    grid_data_1<= 16'h0000;
	  else
			    grid_data_1<= 16'hffff;
				 
	  if ((x_cnt[6]==1'b1) ^ (y_cnt[6]==1'b1))            //Создание изображения большой сетки
			    grid_data_2<=16'h0000;
	  else
				 grid_data_2<=16'hffff; 
   
	end
	
//----------------------------------------------------------------
////////// Создание тестового изображения цветной полосы
//----------------------------------------------------------------
 always @(negedge vga_clk)   
   begin
     if (x_cnt==Hde_start)            
			    bar_data<= 16'hf800;              //Полоса красного цвета
	  else if (x_cnt==Hde_start + bar_interval)
			    bar_data<= 16'h07e0;              //Полоса зеленого цвета				 
	  else if (x_cnt==Hde_start + bar_interval*2)            
			    bar_data<=16'h001f;               //Полоса синего цвета
	  else if (x_cnt==Hde_start + bar_interval*3)         
			    bar_data<=16'hf81f;               //Полоса фиолетового цвета
	  else if (x_cnt==Hde_start + bar_interval*4)           
			    bar_data<=16'hffe0;               //Желтая цветная полоса
	  else if (x_cnt==Hde_start + bar_interval*5)            
			    bar_data<=16'h07ff;               //Полоса голубого цвета
	  else if (x_cnt==Hde_start + bar_interval*6)             
			    bar_data<=16'hffff;               //Полоса белого цвета
	  else if (x_cnt==Hde_start + bar_interval*7)            
			    bar_data<=16'hfc00;               //Полоса оранжевого цвета
	  else if (x_cnt==Hde_start + bar_interval*8)              
			    bar_data<=16'h0000;               //Остальное черное
   
	end
	
//----------------------------------------------------------------
////////// VGA Вывод выбора изображения
//----------------------------------------------------------------
 //LCD Выбор сигнала данных
 always @(negedge vga_clk)  
    if(~rstn) begin 
	    vga_r_reg<=0; 
	    vga_g_reg<=0;
	    vga_b_reg<=0;		 
	end
   else
     case(vga_dis_mode)
         4'b0000:begin
			        vga_r_reg<=0;                        //VGA Дисплей весь черный
                 vga_g_reg<=0;
                 vga_b_reg<=0;
			end
			4'b0001:begin
			        vga_r_reg<=5'b11111;                 //VGA Показать все белое
                 vga_g_reg<=6'b111111;
                 vga_b_reg<=5'b11111;
			end
			4'b0010:begin
			        vga_r_reg<=5'b11111;                 //VGA Показать все красное
                 vga_g_reg<=0;
                 vga_b_reg<=0;  
         end			  
	      4'b0011:begin
			        vga_r_reg<=0;                        //VGA Показать все зеленым
                 vga_g_reg<=6'b111111;
                 vga_b_reg<=0; 
         end					  
         4'b0100:begin     
			        vga_r_reg<=0;                        //VGA Показать все синим
                 vga_g_reg<=0;
                 vga_b_reg<=5'b11111;
			end
         4'b0101:begin     
			        vga_r_reg<=grid_data_1[15:11];       // VGA Показать блок 1
                 vga_g_reg<=grid_data_1[10:5];
                 vga_b_reg<=grid_data_1[4:0];
         end					  
         4'b0110:begin     
			        vga_r_reg<=grid_data_2[15:11];       // VGA Показать блок 2
                 vga_g_reg<=grid_data_2[10:5];
                 vga_b_reg<=grid_data_2[4:0];
			end
		   4'b0111:begin     
			        vga_r_reg<=x_cnt[6:2];               //VGA Показать горизонтальный градиент
                 vga_g_reg<=x_cnt[6:1];
                 vga_b_reg<=x_cnt[6:2];
			end
		   4'b1000:begin     
			        vga_r_reg<=y_cnt[6:2];               //VGA Показать вертикальный градиент
                 vga_g_reg<=y_cnt[6:1];
                 vga_b_reg<=y_cnt[6:2];
			end
		   4'b1001:begin     
			        vga_r_reg<=x_cnt[6:2];               //VGA Отобразить красный горизонтальный градиент
                 vga_g_reg<=0;
                 vga_b_reg<=0;
			end
		   4'b1010:begin     
			        vga_r_reg<=0;                        //VGA Показать зеленый горизонтальный градиент
                 vga_g_reg<=x_cnt[6:1];
                 vga_b_reg<=0;
			end
		   4'b1011:begin     
			        vga_r_reg<=0;                        //VGA Показать синий горизонтальный градиент
                 vga_g_reg<=0;
                 vga_b_reg<=x_cnt[6:2];			
			end
		   4'b1100:begin     
			        vga_r_reg<=bar_data[15:11];          //VGA Показать цветные полосы
                 vga_g_reg<=bar_data[10:5];
                 vga_b_reg<=bar_data[4:0];			
			end
		   default:begin
			        vga_r_reg<=5'b11111;                 //VGA Показать все белое
                 vga_g_reg<=6'b111111;
                 vga_b_reg<=5'b11111;
			end					  
         endcase
	

  assign vga_hs = hsync_r;
  assign vga_vs = vsync_r;  
  assign vga_r = (hsync_de & vsync_de)?vga_r_reg:5'b00000;
  assign vga_g = (hsync_de & vsync_de)?vga_g_reg:6'b000000;
  assign vga_b = (hsync_de & vsync_de)?vga_b_reg:5'b00000;
  
 //производить 65Mhz VGA Clock  
   pll pll_inst
  (
   .inclk0(clk),               
   .c0(vga_clk),               // 65.0Mhz for 1024x768(60hz)
   .areset(~rstn),              
   .locked()
	);              


  //Обработчик кнопок	
  always @(posedge vga_clk)
  begin
    if(~rstn) begin 
	    vga_dis_mode<=4'b0000; 
		 key1_counter<=0;	 
	 end	
	 else begin
	    if (key1==1'b1)                               //Если кнопка не нажата, регистр равен 0
	       key1_counter<=0;
	    else if ((key1==1'b0)& (key1_counter<=20'd90_000)) //Если кнопка нажата и удерживается менее 1 мс, считать(9M*0.1=900_000)     
          key1_counter<=key1_counter+1'b1;
  	  
       if (key1_counter==20'd89_999)                //Одна кнопка эффективна, измените режим отображения
		    begin
		      if(vga_dis_mode==4'b1101)
			      vga_dis_mode<=4'b0000;
			   else
			      vga_dis_mode<=vga_dis_mode+1'b1; 
          end	
     end		
  end	
	  

endmodule
