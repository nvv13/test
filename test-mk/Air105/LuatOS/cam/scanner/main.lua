

PROJECT = "scanner"
VERSION = "1.0.0"

sys = require("sys")

--[[
-- LCD Connection example, take the HSPI of the Air105 development board as an example
LCD pins Air105 pins
GND      GND
VCC      3.3V
SCL      (PC15/HSPI_SCK)
SDA      (PC13/HSPI_MOSI)
RES      (PC12/HSPI_MISO)
DC       (PE08) --U3_RX on the development board
CS       (PC14/HSPI_CS)
BL       (PE09) --U3_TX on the development board


hint:
1. Only use SPI clock line (SCK) and data output line (MOSI), others are GPIO pins
2. Although data input (MISO) and chip select (CS) are SPI, they have been multiplexed as GPIO, which are not fixed and can be freely modified to other pins.

-- USB driver download https://doc.openluat.com/wiki/21?wiki_page_id=2070
-- The USB driver is consistent with the USB driver of Hezhou Cat.1

Scan code decoding will output logs, and also output to USB HID.

If you need to view via USB HID
1. To install the USB driver
2. After flashing the script, be sure to download the txt as well, a total of 3 files.
3. Turn off the power, flip the toggle switch, and reconnect the computer
4. Open Notepad, the cursor is in the input state
5. Move the camera to scan the code, the result will be output to Notepad, once per line

]]

if wdt then
    wdt.init(9000)--initialize watchdog set to 9s
    sys.timerLoopStart(wdt.feed, 3000)--3s feed the dog once
end

spi_lcd = spi.deviceSetup(5,pin.PC14,1,1,8,48*1000*1000,spi.MSB,1,1) -- st7789 CPHA=1 CPOL=1 current controller
--spi_lcd = spi.deviceSetup(5,pin.PC14,0,0,8,48*1000*1000,spi.MSB,1,1) -- st7735s

--log.info("lcd.init",
--lcd.init("st7735s",{port = "device",pin_dc = pin.PE08 ,pin_rst = pin.PC12,pin_pwr = pin.PE09,direction = 2,w = 160,h = 80,xoffset = 1,yoffset = 26},spi_lcd))

log.info("lcd.init",
lcd.init("st7789",{port = "device",pin_dc = pin.PE08 ,pin_rst = pin.PC12,pin_pwr = pin.PE09,direction = 0,w = 240,h = 240,xoffset = 0,yoffset = 0},spi_lcd))

-- log.info("lcd.init",
-- lcd.init("st7735",{port = "device",pin_dc = pin.PE08 ,pin_rst = pin.PC12,pin_pwr = pin.PE09,direction = 0,w = 128,h = 160,xoffset = 2,yoffset = 1},spi_lcd))

-- log.info("lcd.init",
-- lcd.init("gc9306x",{port = "device",pin_dc = pin.PE08 ,pin_rst = pin.PC12,pin_pwr = pin.PE09,direction = 0,w = 240,h = 320,xoffset = 0,yoffset = 0},spi_lcd))

--GC032AOutput grayscale image initialization command
local GC032A_InitReg_Gray =
{
	zbar_scan = 1,--Whether to scan the code
    draw_lcd = 1,--whether to output to lcd
    i2c_id = 0,
	i2c_addr = 0x21,
    pwm_id = 5;
    pwm_period  = 24*1000*1000,
    pwm_pulse = 0,
	sensor_width = 640,
	sensor_height = 480,
    color_bit = 16,
	init_cmd ="/luadb/GC032A_InitReg_Gray.txt"--This method writes the initialization instruction in an external file and supports the use of # for comments
}

--GC032A output rgb image initialization command
local GC032A_InitReg =
{
	zbar_scan = 0,--Whether to scan the code
    draw_lcd = 1,--whether to output to lcd
    i2c_id = 0,
	i2c_addr = 0x21,
    pwm_id = 5;
    pwm_period  = 12*1000*1000,
    pwm_pulse = 0,
	sensor_width = 640,
	sensor_height = 480,
    color_bit = 16,
	init_cmd ="/luadb/GC032A_InitReg.txt"--This method writes the initialization instruction in an external file and supports the use of # for comments
}

--Register camera event callback
local tick_scan = 0
camera.on(0, "scanned", function(id, str)
    if type(str) == 'string' then
        log.info("scan result", str)
        -- Air105 only needs 200ms to scan the code each time. When the target 1D code or 2D code is continuously recognized, this function will be triggered repeatedly
        -- In view of some scenarios that require interval output, the following code is to demonstrate the interval output
        -- if mcu.ticks() - tick < 1000 then
        -- return
        -- end
        -- tick_scan = mcu.ticks()
        -- The output content can be processed and output, for example, with a newline (Enter key)
        usbapp.vhid_upload(0, str.."\r\n")
    elseif str == false then
        log.error("camera has no data")
    end
end)

local camera_pwdn = gpio.setup(pin.PD06, 1, gpio.PULLUP) -- PD06 camera_pwdn pin
local camera_rst = gpio.setup(pin.PD07, 1, gpio.PULLUP) -- PD07 camera_rst pin

usbapp.start(0)

sys.taskInit(function()
    camera_rst(0)
    --local camera_id = camera.init(GC032A_InitReg_Gray)--Screen output grayscale image and scan code
    local camera_id = camera.init(GC032A_InitReg)--Screen output grayscale image and scan code

    log.info("camera start")
    camera.start(camera_id)--Start the specified camera
end)

-- end of user code -------------------------------------------- -
-- Always end with this sentence
sys.run()
-- sys.run()Do not add any statement after that!!!!!
