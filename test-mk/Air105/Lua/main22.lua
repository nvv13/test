PROJECT = "lcddemo"
VERSION = "1.0.0"

log.info("main", PROJECT, VERSION)
_G.sys = require("sys")

if wdt then
    wdt.init(9000)
    sys.timerLoopStart(wdt.feed, 3000)
end

local spi_id,pin_reset,pin_dc,pin_cs,bl = 5,pin.PC12,pin.PE08,pin.PC14,pin.PE09

spi_lcd = spi.deviceSetup(5,pin.PC14,0,0,8,24*1000*1000,spi.MSB,1,0)

lcd.init("custom",{
    port = "device",
    pin_dc = pin_dc, 
    pin_pwr = bl,
    pin_rst = pin_reset,
    direction = 3,
    w = 80,
    h = 160,
    xoffset = 24,
    yoffset = 0,
    sleepcmd = 0x10,
    wakecmd = 0x11,
    initcmd = {
        0x00020011, --: Sleep Out
		0x00010078,
		------------------------------------ST7735S Frame Rate-----------------------------------------// 
		0x000200B1, --Frame Rate Control (In normal mode/ Full colors)
		0x00030005,
		0x0003003C,
		0x0003003C,
		0x000200B2, --Frame Rate Control (In Idle mode/ 8-colors)
		0x00030005,
		0x0003003C,
		0x0003003C,
		0x000200B3, --Frame Rate Control (In Partial mode/ full colors)
		0x00030005,
		0x0003003C,
		0x0003003C,
		0x00030005,
		0x0003003C,
		0x0003003C,
		------------------------------------End ST7735S Frame Rate---------------------------------// 
		0x000200B4, --Display Inversion Control
		0x00030000, 
		------------------------------------ST7735S Power Sequence---------------------------------// 
		0x000200C0, --Power Control 1
		0x00030028,
		0x00030008,
		0x00030004,
		0x000200C1, --Power Control 2
		0x000300C0,
		0x000200C2, --Power Control 3 (in Normal mode/ Full colors)
		0x0003000D,
		0x00030000,
		0x000200C3, --Power Control 4 (in Idle mode/ 8-colors)
		0x0003008D,
		0x0003002A,
		0x000200C4, --Power Control 5 (in Partial mode/ full-colors)
		0x0003008D,
		0x000300EE,
		---------------------------------End ST7735S Power Sequence-------------------------------------// 
		0x000200C5, --VCOM Control 1
		0x0003000E,
		0x00020036,--Memory Data Access Control
		0x000300C0, 
		------------------------------------ST7735S Gamma Sequence---------------------------------// 
		0x000200E0, --Gamma (‘+’polarity) Correction Characteristics Setting
		0x00030004,
		0x00030022,
		0x00030007,
		0x0003000A,
		0x0003002E,
		0x00030030,	
		0x00030025,
		0x0003002A,
		0x00030028,
		0x00030026,
		0x0003002E,
		0x0003003A,
		0x00030000,
		0x00030001,
		0x00030003,
		0x00030013,
		0x000200E1, --Gamma ‘-’polarity Correction Characteristics Setting 
		0x00030004,
		0x00030016,
		0x00030006,
		0x0003000D,
		0x0003002D,
		0x00030026,
		0x00030023,
		0x00030027,
		0x00030027,
		0x00030025,
		0x0003002D,
		0x0003003B,
		0x00030000,
		0x00030001,
		0x00030004,
		0x00030013,
		--------------------------------------End ST7735S Gamma Sequence-----------------------------// 
		0x0002003A, --Interface Pixel Format --65k mode 
		0x00030005, 
		--------------------------------------orent disp grad-----------------------------// 
		0x00020036, -- orent disp grad
                0x000300A8, -- direction=3 270
		0x00020029, --Display On
                ---------------USE CONFIG-------------
                0x0002002A, -- Set Column Address
                0x00030000,  
                0x00030018, --  xoffset = 24 
                0x00030000,  
                0x00030067, --  w+ xoffset-1   ( 80 + 24 - 1 =  103 )
                0x0002002B, -- Set Page Address
                0x00030000, 
                0x00030000, -- yoffset  = 0
                0x00030000, 
                0x0003009F, -- h + yoffset-1  ( 160 + 0 - 1 = 159 )
                0x0002002C, 
    },
    },
    spi_lcd)


--lcd.init("st7735s" ,{port = "device",pin_dc = pin_dc, pin_pwr = bl, pin_rst = pin_reset, direction = 0,w = 160, h = 80,xoffset = 24,yoffset = 0},spi_lcd)

sys.taskInit(function() 
	--sys.wait(500)
	--lcd.setColor(0xffff,0x001f)
	--lcd.invoff()
	--lcd.clear(0x0000)
	--lcd.flush()
	lcd.clear(0x0)
	--lcd.clear(0x0000)

	--lcd.drawLine(40,0,40,159,0x001f)

	for y=0,160 do
		lcd.drawStr(0,y,tostring(y),0x001f)
		sys.wait(50)
		--lcd.drawStr(0,y,"0123456789",0xffff)
		lcd.clear(0x0)
	end
	--lcd.drawQrcode(0,0,"str",100)
	--lcd.drawStr(0,80,"1234567890",0x001F)
	--sys.wait(1000)
	
	--log.info("lcd.drawLine", lcd.drawLine(20, 20, 150, 20, 0x001F))
	--log.info("lcd.drawRectangle", lcd.drawRectangle(20, 40, 120, 70, 0xF800))
	--log.info("lcd.drawCircle", lcd.drawCircle(50, 50, 20, 0x0CE0))
end)

sys.run()
