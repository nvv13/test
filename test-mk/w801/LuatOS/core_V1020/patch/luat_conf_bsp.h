
#ifndef LUAT_CONF_BSP
#define LUAT_CONF_BSP

#define LUAT_BSP_VERSION "V1020"

// The Flash size of Air101 and Air103 is different, need to distinguish
#define AIR101

// Enable 64-bit virtual machine
// #define LUAT_CONF_VM_64bit



// FLASH_FS_REGION_SIZE contains 2 parts: script area and file system area
// Among them, the file system area is fixed at 48k, and the script area defaults to 64k. The sum of the two is the default value 64+48=112
// If you need to increase the size of the script area, the size must be an integer multiple of 64, for example, it becomes 128,192,256
// 128k script area, the corresponding FLASH_FS_REGION_SIZE is 176, because 128+48=176
// 192k script area, the corresponding FLASH_FS_REGION_SIZE is 240, because 192+48=240
// 256k script area, the corresponding FLASH_FS_REGION_SIZE is 304, because 256+48=304
#define LUAT_FS_SIZE                48      //file system size
#define LUAT_SCRIPT_SIZE            64      //script size
#define FLASH_FS_REGION_SIZE        (LUAT_FS_SIZE + LUAT_SCRIPT_SIZE)

// Memory optimization: reduce memory consumption, slightly reduce performance
// #define LUAT_USE_MEMORY_OPTIMIZATION_CODE_MMAP 1

//----------------------------------
// To use VFS (Virtual File System) and built-in library files, it must be enabled
#define LUAT_USE_FS_VFS 1
#define LUAT_USE_VFS_INLINE_LIB 1
//----------------------------------

//----------------------------
// Peripherals, enable on demand, at least enable uart and wdt libraries
#define LUAT_USE_UART 1
#define LUAT_USE_GPIO 1
#define LUAT_USE_I2C  1
#define LUAT_USE_SPI  1
#define LUAT_USE_ADC  1
#define LUAT_USE_PWM  1
#define LUAT_USE_WDT  1
#define LUAT_USE_PM  1
#define LUAT_USE_MCU  1
#define LUAT_USE_RTC 1
// SDIO only supports TF/SD card mounting
#define LUAT_USE_SDIO 1
// Segment screen/segment screen, enable on demand
#define LUAT_USE_LCDSEG 1
// OTP
#define LUAT_USE_OTP 1
#define LUAT_USE_TOUCHKEY 1

// #define LUAT_USE_ICONV 1

// wlan library related
#define LUAT_USE_WLAN
#define LUAT_USE_LWIP
#define LUAT_USE_NETWORK
#define LUAT_USE_DNS
#define LUAT_USE_SNTP
#define LUAT_USE_HTTPSRV
#define LUAT_USE_FTP

// External network support
// #define LUAT_USE_W5500_XXX
// #define LUAT_USE_DHCP

// enable TLS
// #define LUAT_USE_TLS


#define LUAT_USE_IOTAUTH 1

//----------------------------
// Common tool library, enabled on demand, cjson and pack are strongly recommended to enable
#define LUAT_USE_CRYPTO  1
#define LUAT_USE_CJSON  1
#define LUAT_USE_ZBUFF  1
#define LUAT_USE_PACK  1
#define LUAT_USE_LIBGNSS  1
#define LUAT_USE_FS  1
#define LUAT_USE_SENSOR  1
#define LUAT_USE_SFUD  1
// #define LUAT_USE_STATEM 1
// Performance test, just play after running, don't enable it together with lvgl, don't add this library to the firmware of the production environment
// #define LUAT_USE_COREMARK 1
// #define LUAT_USE_ZLIB 1
#define LUAT_USE_IR 1
// FDB provides kv database, similar to nvm library
// #define LUAT_USE_FDB 1
// fskv provides an API compatible with the fdb library, designed to replace the fdb library
#define LUAT_USE_FSKV 1
#define LUAT_USE_OTA 1
#define LUAT_USE_I2CTOOLS 1
#define LUAT_USE_LORA 1
#define LUAT_USE_MLX90640 1
// #define LUAT_USE_MAX30102 1
// zlib compression, faster and smaller implementation
#define LUAT_USE_MINIZ 1
// The memory requirement of FASTLZ is small, and the compression ratio is not as good as miniz
// #define LUAT_USE_FASTLZ 1

// RSA encryption and decryption, signature verification
#define LUAT_USE_RSA 1

// National secret algorithm SM2/SM3/SM4
#define LUAT_USE_GMSSL 1


// // Use TLSF memory pool, experimental, higher memory utilization
// #define LUAT_USE_TLSF 1

// Disable the hardware accelerated version of AES, use the software version of mbedtls instead
// The hardware accelerated version only supports AES128, if AES256 is used, an error will be reported
#define LUAT_USE_CRYPTO_AES_MBEDTLS  1

#define LUAT_USE_MEDIA  1

//---------------SDIO-FATFS special configuration
// The sdio library is connected to fatfs
// The long file name and non-English file name support of fatfs requires 180k ROM, which is very luxurious
// Start from v0006 and turn it off by default, turn it on if you need it
#define LUAT_USE_FATFS
// #define LUAT_USE_FATFS_CHINESE

//----------------------------
// Qualcomm font, need to be used with the chip
// #define LUAT_USE_GTFONT 1
// #define LUAT_USE_GTFONT_UTF8

// #define LUAT_USE_YMODEM

//----------------------------
// For advanced functions, it is recommended to use REPL, because SHELL is obsolete
// #define LUAT_USE_SHELL 1
// #define LUAT_USE_DBG
// NIMBLE is a Bluetooth function, called BLE, but not low power consumption.
// #define LUAT_USE_NIMBLE 1
// Multi-virtual machine support, experimental, generally not enabled
// #define LUAT_USE_VMX 1
// #define LUAT_USE_NES
#define LUAT_USE_PROTOBUF 1
#define LUAT_USE_REPL 1

//---------------------
// UI
// LCD is a color screen, if you use LVGL, you must enable LCD
#define LUAT_USE_LCD
#define LUAT_USE_TJPGD
// EINK is ink screen
#define LUAT_USE_EINK

//---------------------
// U8G2
// Monochrome screen, support i2c/spi
#define LUAT_USE_DISP
#define LUAT_USE_U8G2

/**************FONT*****************/
#define LUAT_USE_FONTS
/**********U8G2&LCD&EINK FONT*************/
#define USE_U8G2_OPPOSANSM_ENGLISH 1
// #define USE_U8G2_OPPOSANSM8_CHINESE
#define USE_U8G2_OPPOSANSM10_CHINESE
#define USE_U8G2_OPPOSANSM12_CHINESE
// #define USE_U8G2_OPPOSANSM16_CHINESE
// #define USE_U8G2_OPPOSANSM24_CHINESE
// #define USE_U8G2_OPPOSANSM32_CHINESE
// SARASA
// #define USE_U8G2_SARASA_M8_CHINESE
// #define USE_U8G2_SARASA_M10_CHINESE
// #define USE_U8G2_SARASA_M12_CHINESE
// #define USE_U8G2_SARASA_M14_CHINESE
// #define USE_U8G2_SARASA_M16_CHINESE
// #define USE_U8G2_SARASA_M18_CHINESE
// #define USE_U8G2_SARASA_M20_CHINESE
// #define USE_U8G2_SARASA_M22_CHINESE
// #define USE_U8G2_SARASA_M24_CHINESE
// #define USE_U8G2_SARASA_M26_CHINESE
// #define USE_U8G2_SARASA_M28_CHINESE
/**********LVGL FONT*************/
// #define LV_FONT_OPPOSANS_M_8
// #define LV_FONT_OPPOSANS_M_10
// #define LV_FONT_OPPOSANS_M_12
// #define LV_FONT_OPPOSANS_M_16

// -------------------------------------
//PSRAM
// An external PSRAM chip is required, otherwise do not enable it, it will crash
// Although air101 supports psram, there is a multiplexing conflict with spi
// air103 supports the use of psram and spi at the same time, and the multiplexing does not conflict
// #define LUAT_USE_PSRAM 1
// LVGL recommends putting some methods into memory and using them on demand
// #define LV_ATTRIBUTE_FAST_MEM __attribute__((section (".ram_run")))
// ROTABLE technology is the key technology to save memory. There is no shortage of memory after enabling PSRAM, and disabling it can improve performance
// #define LUAT_CONF_DISABLE_ROTABLE 1
//---------------------------------------


//---------------------
// LVGL
// The main UI library, powerful but cumbersome API
#define LUAT_USE_LVGL
#define LV_DISP_DEF_REFR_PERIOD 30
#define LUAT_LV_DEBUG 0

#define LV_MEM_CUSTOM 1

#define LUAT_USE_LVGL_INDEV 1 // input device

#define LUAT_USE_LVGL_ARC //Arc no dependency
#define LUAT_USE_LVGL_BAR // progress bar no dependencies
#define LUAT_USE_LVGL_BTN //Button depends on container CONT
#define LUAT_USE_LVGL_BTNMATRIX //button matrix no dependencies
#define LUAT_USE_LVGL_CALENDAR //calendar no dependencies
#define LUAT_USE_LVGL_CANVAS //canvas depends on image IMG
#define LUAT_USE_LVGL_CHECKBOX // check box depends on button BTN label LABEL
#define LUAT_USE_LVGL_CHART //chart no dependencies
#define LUAT_USE_LVGL_CONT //container without dependencies
#define LUAT_USE_LVGL_CPICKER //color selector no dependencies
#define LUAT_USE_LVGL_DROPDOWN //The drop-down list depends on the page PAGE label LABEL
#define LUAT_USE_LVGL_GAUGE //Gauge depends on the progress bar BAR gauge (arc scale) LINEMETER
#define LUAT_USE_LVGL_IMG //Image depends on label LABEL
#define LUAT_USE_LVGL_IMGBTN //Image button depends on button BTN
#define LUAT_USE_LVGL_KEYBOARD //Keyboard depends on image button IMGBTN
#define LUAT_USE_LVGL_LABEL //label no dependency
#define LUAT_USE_LVGL_LED // LED has no dependencies
#define LUAT_USE_LVGL_LINE //line no dependencies
#define LUAT_USE_LVGL_LIST //List dependent page PAGE button BTN label LABEL
#define LUAT_USE_LVGL_LINEMETER //Meter (arc scale) no dependencies
#define LUAT_USE_LVGL_OBJMASK //Object mask without dependency
#define LUAT_USE_LVGL_MSGBOX //The message box depends on the image button IMGBTN label LABEL
#define LUAT_USE_LVGL_PAGE //page depends on container CONT
#define LUAT_USE_LVGL_SPINNER //rotator depends on arc ARC animation ANIM
#define LUAT_USE_LVGL_ROLLER //Roller has no dependencies
#define LUAT_USE_LVGL_SLIDER //The slider depends on the progress bar BAR
#define LUAT_USE_LVGL_SPINBOX //Number adjustment box without dependency
#define LUAT_USE_LVGL_SWITCH //Switch depends on slider SLIDER
#define LUAT_USE_LVGL_TEXTAREA //text box depends on label LABEL page PAGE
#define LUAT_USE_LVGL_TABLE //Table dependent label LABEL
#define LUAT_USE_LVGL_TABVIEW //Tab dependent page PAGE picture button IMGBTN
#define LUAT_USE_LVGL_TILEVIEW //Tile view depends on page PAGE
#define LUAT_USE_LVGL_WIN //Window dependent container CONT button BTN label LABEL image IMG page PAGE

#define LV_HOR_RES_MAX          (160)
#define LV_VER_RES_MAX          (80)
#define LV_COLOR_DEPTH          16

#define LV_COLOR_16_SWAP   1

#define LUAT_RET int
#define LUAT_RT_RET_TYPE	void
#define LUAT_RT_CB_PARAM void *param

// Macros purely for generating documentation
#define LUAT_USE_PIN 1

#define LUAT_GPIO_PIN_MAX (48)

#endif
