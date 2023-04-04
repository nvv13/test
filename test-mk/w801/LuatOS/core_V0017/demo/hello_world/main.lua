-- LuaTools The two information of PROJECT and VERSION are required
PROJECT = "wifidemo"
VERSION = "1.0.0"

-- Introduce necessary library files (written in lua), internal libraries do not require require
sys = require("sys")
require("sysplus")

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

sys.taskInit(function()
    sys.wait(1000)
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
            local code, headers, body = http.request("GET", url).wait()
            log.info("http", code, json.encode(headers or {}), body and #body or 0)
            sys.wait(5000)
            log.info("lua", rtos.meminfo())
            log.info("sys", rtos.meminfo("sys"))
            sys.wait(1000)
        end
    else
        print("wlan NOT ready!!!!")
    end
end)

-- Usercode has ended---------------------------------------------
-- always ends with this sentence
sys.run()
-- sys.run()Do not add any statement after that!!!!!
