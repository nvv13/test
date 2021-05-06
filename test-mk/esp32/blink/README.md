blink from esp12f

   text    data     bss     dec
 284156    5264   31816  321236

flash ROM = 284156 + 5264 = 289420
      RAM = 5264 + 31816  = 37080



/home/nvv/RIOT/dist/tools/esptool/esptool.py --chip esp32 --port /dev/ttyACM0 --baud 115200 --before default_reset --after hard_reset erase_flash

/home/nvv/RIOT/dist/tools/esptool/esptool.py --chip esp32 --port /dev/ttyCH343USB0 --baud 115200 --before default_reset --after hard_reset erase_flash
