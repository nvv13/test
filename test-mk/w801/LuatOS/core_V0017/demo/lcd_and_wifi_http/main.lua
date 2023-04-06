-- LuaTools The two information of PROJECT and VERSION are required
PROJECT = "wifidemo"
VERSION = "1.0.0"

-- Introduce necessary library files (written in lua), internal libraries do not require require
sys = require("sys")
require("sysplus")


--[[ 
-- LCD Connection для spi_id = 1
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

-- как найти сайты без https в Google? пример > море -inurl:https
-- найдёт сайты по ключевому слову «море», в адресе которых отсутствует "https"


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
    --wlan.connect("bred1", "9115676369")
    wlan.connect("bred8", "123123123")
    log.info("wlan", "wait for IP_READY")
    sys.waitUntil("IP_READY", 30000)
    if wlan.ready() then
        log.info("wlan", "ready !!")
        sys.wait(100)
        local url = "http://time.jsontest.com"
        while 1 do
            lcd.clear(0x0F0F)
            local code, headers, body = http.request("GET", url).wait()
            local s_dat=json.encode(headers or {})
            log.info("http", code, s_dat, body and #body or 0)
            lcd.setFont(lcd.font_opposansm18)
            local s_pt_date=s_dat:sub(s_dat:find("Date")+7,s_dat:find("GMT",s_dat:find("Date"))+2);
            log.info("Date",s_pt_date)            
            local s_time=s_pt_date:sub(18,25);
            local i_hour=tonumber(s_time:sub(1,2))+3
            local i_min=tonumber(s_time:sub(4,5))
            local i_sec=tonumber(s_time:sub(7,8))
            if i_hour>23 then 
              i_hour=i_hour-24 
            end
            local s_pt_time=string.format("%.2d:%.2d:%.2d",i_hour,i_min,i_sec)
            log.info("Time",s_pt_time)            
            lcd.drawStr(40,40,s_pt_time,0x0000)
            sys.wait(5000)
            log.info("rtos.meminfo", rtos.meminfo())
            log.info("rtos.meminfo lua", rtos.meminfo("lua"))
            log.info("rtos.meminfo sys", rtos.meminfo("sys"))
            log.info("rtos.bsp", rtos.bsp())
            log.info("rtos.buildDate", rtos.buildDate())
            log.info("rtos.version", rtos.version())
            log.info("rtosfirmware", rtos.firmware())
            log.info("os.date", os.date())
            log.info("mcu.unique_id", string.toHex(mcu.unique_id()))
            log.info("mcu.getClk", mcu.getClk())
            log.info("mcu.ticks", mcu.ticks())
            if rtc == nil then
             log.info("this fireware is not support rtc")
            else
             --rtc.set({year = 2021, mon = 2, day = 20, hour = 0, min = 0, sec = 0})
             log.info("rtc.get", json.encode(rtc.get()))
            end

    if socket == nil then
        log.info( "this fireware is not support socket")
    end
    if spi == nil then
        log.info( "this fireware is not support spi")
    end
    if statem == nil then
        log.info( "this fireware is not support statem")
    end
    if sfud == nil then
        log.info( "this fireware is not support sfud")
    end
    if sfd == nil then
        log.info( "this fireware is not support sfd")
    end
    if sdio == nil then
        log.info( "this fireware is not support sdio")
    end
    if pwm == nil then
        log.info( "this fireware is not support pwm")
    end
    if pin == nil then
        log.info( "this fireware is not support pin")
    end
    if pack == nil then
        log.info( "this fireware is not support pack")
    end
    if i2c == nil then
        log.info( "this fireware is not support i2c")
    end
    if gpio == nil then
        log.info( "this fireware is not support gpio")
    end
    if fs == nil then
        log.info( "this fireware is not support fs")
    end
    if fdb == nil then
        log.info( "this fireware is not support fdb")
    end
    if espnow == nil then
        log.info( "this fireware is not support espnow")
    end
    if esphttp == nil then
        log.info( "this fireware is not support esphttp")
    end
    if eink == nil then
        log.info( "this fireware is not support eink")
    end
    if crypto == nil then
        log.info( "this fireware is not support crypto")
    end
    if camera == nil then
        log.info( "this fireware is not support camera")
    end
    if ble == nil then
        log.info( "this fireware is not support ble")
    end
    if adc == nil then
        log.info( "this fireware is not support adc")
    end
    if zlib == nil then
        log.info( "this fireware is not support zlib")
    end
    if zbuff == nil then
        log.info( "this fireware is not support zbuff")
    end
    if wlan == nil then
        log.info( "this fireware is not support wlan")
    end
    if wdt == nil then
        log.info( "this fireware is not support wdt")
    end
    if uart == nil then
        log.info( "this fireware is not support uart")
    end
    if u8g2 == nil then
        log.info( "this fireware is not support u8g2")
    end
    if touchkey == nil then
        log.info( "this fireware is not support touchkey")
    end
    if sys == nil then
        log.info( "this fireware is not support sys")
    end
    if string == nil then
        log.info( "this fireware is not support string")
    end


            --lcd.drawStr(40,80,rtos.bsp():lower(),0x00F0)
            lcd.drawStr(40,80,mcu.ticks(),0x00F0)
            lcd.drawXbm(80,100, 16,16, string.char(
              0x80,0x00,0x80,0x00,0x40,0x01,0x20,0x02,0x10,0x04,0x48,0x08,0x84,0x10,0x83,0x60,
              0x00,0x00,0xF8,0x0F,0x00,0x08,0x00,0x04,0x00,0x04,0x00,0x02,0x00,0x01,0x80,0x00
                                ))
            sys.wait(1000)
            lcd.showImage(0,0,"/luadb/dacha.jpg")
            sys.wait(10000)
        end
    else
        print("wlan NOT ready!!!!")
    end
end)



-- Usercode has ended---------------------------------------------
-- always ends with this sentence
sys.run()
-- sys.run()Do not add any statement after that!!!!!
