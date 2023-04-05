-- LuaTools The two information of PROJECT and VERSION are required
PROJECT = "wifidemo"
VERSION = "1.0.0"

-- Introduce necessary library files (written in lua), internal libraries do not require require
sys = require("sys")
require("sysplus")


--[[
-- LCD Connection example
LCD pins W801 pins
GND      GND
VCC      3.3V
SCL      PB15
SDA      PB17
RES      PB21
DC       PB23
CS       PB14
BL       PB24
]]



-- sys.subscribe("WLAN_READY", function ()
--     print("!!! wlan ready event !!!")
-- end)

-- sys.taskInit(function()
--     while 1 do
--         sys.wait(5000)
--         log.info("lua", rtos.meminfo())
--         log.info("sys", rtos.meminfo("sys"))
--     end

-- end)

-- Compatible with V0017 firmware
if http == nil and http2 then
    http = http2
end

spi_lcd = spi.deviceSetup(1,pin.PB14,1,1,8,48*1000*1000,spi.MSB,1,1) -- st7789 CPHA=1 CPOL=1 current controller

log.info("lcd.init",
lcd.init("st7789",{port = "device",pin_dc = pin.PB23 ,pin_rst = pin.PB21,pin_pwr = pin.PB24,direction = 0,w = 240,h = 240,xoffset = 0,yoffset = 0},spi_lcd))


sys.taskInit(function()
    sys.wait(1000)
     lcd.on()
    wlan.init()
    wlan.connect("bred1", "9115676369")
    log.info("wlan", "wait for IP_READY")
    sys.waitUntil("IP_READY", 30000)
    if wlan.ready() then
        log.info("wlan", "ready !!")
        sys.wait(100)
        -- local url = "http://time.jsontest.com"
        local url = "http://time.jsontest.com"
        while 1 do
            lcd.clear(0x0F0F)
            local code, headers, body = http.request("GET", url).wait()
            log.info("http", code, json.encode(headers or {}), body and #body or 0)
            lcd.setFont(lcd.font_opposansm18)
            lcd.drawStr(0,40,json.encode(headers or {}):sub(175,190))
            sys.wait(5000)
            --lcd.clear(0xF0FF)
            log.info("lua", rtos.meminfo())
            log.info("sys", rtos.meminfo("sys"))
            log.info("bsp", rtos.bsp():lower())
            lcd.drawStr(40,80,rtos.bsp():lower())
            sys.wait(5000)
        end
    else
        print("wlan NOT ready!!!!")
    end
end)

-- Usercode has ended---------------------------------------------
-- always ends with this sentence
sys.run()
-- sys.run()Do not add any statement after that!!!!!
