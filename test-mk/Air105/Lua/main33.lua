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
    direction = 0,
    w = 80,
    h = 160,
    xoffset = 24,
    yoffset = 0,
    sleepcmd = 0x10,
    wakecmd = 0x11,
    initcmd = {
		0x000200fe,
		0x000200fe,
		0x000200fe,
		0x000200ef,

		0x000200b3,
		0x00030003,

		0x00020036,
		0x000300d8,

		0x0002003a,
		0x00030005,

		0x000200b6,
		0x00030011,
		0x000200ac,
		0x0003000b,

		0x000200b4,
		0x00030021,

		0x000200b1,
		0x000300c0,

		0x000200e6,
		0x00030050,
		0x00030043,
		0x000200e7,
		0x00030056,
		0x00030043,

		0x000200F0,
		0x0003001f,
		0x00030041,
		0x0003001B,
		0x00030055,
		0x00030036,
		0x0003003d,
		0x0003003e,
		0x00030000,
		0x00030016,
		0x00030008,
		0x00030009,
		0x00030015,
		0x00030014,
		0x0003000f,

		0x000200F1,

		0x0003001f,
		0x00030041,
		0x0003001B,
		0x00030055,
		0x00030036,
		0x0003003d,
		0x0003003e,
		0x00030000,
		0x00030016,
		0x00030008,
		0x00030009,
		0x00030015,
		0x00030014,
		0x0003000f,

		0x000200fe,
		0x000200ff,

		0x00020035,
		0x00030000,
		0x00020044,
		0x00030000,
		0x00020011,
		0x00010078,

		0x00020029,

		0x0002002A, --Set Column Address
		0x00030000,
		0x00030018,
		0x00030000,
		0x00030067,
		0x0002002B, --Set Page Address
		0x00030000,
		0x00030000,
		0x00030000,
		0x0003009f,
		0x0002002c,

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
