инструкция для платки 

 MilkV-DuoS WiFi

1) скачать образ

  https://github.com/milkv-duo/duo-buildroot-sdk/releases

  ищем 

  milkv-duos-sd-v1.1.4.img.zip

  качаем, 

  записываем на SD, например прогой balena-etcher

2) запускаем, заходим по 

   $ ssh root@192.168.xxx.xxx

         pass milkv

    где

     .xxx.xxx   - адрес что присвоил роутер (dhcp) для Ethernet карты

   я заходил по Ethernet, потому что по USB-CDC, не принимало пароль


3) настраиваем mac адрес чтоб не менялися!

   для Ethernet
~~~
# echo "pre-up ifconfig eth0 hw ether 8a:a0:07:6f:89:e9" >> /etc/network/interfaces  
~~~


4) настраиваем Wifi

   https://community.milkv.io/t/connect-to-wi-fi-on-duo-s/1540

настраиваем ssid и пароль

~~~
# cat >> /etc/wpa_supplicant.conf <<END
update_config=1

network={
	ssid="bred2"
	psk="9115676369"
	key_mgmt=WPA-PSK
}
END
~~~

5) настраиваем автозагрузку WiFi + mac адрес чтоб не менялися

~~~
# cat >> /mnt/system/duo-init.sh <<END
# Auto connect to WIFI on startup
wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf
sleep 0.5
ifconfig wlan0 down
sleep 0.5
ifconfig wlan0 hw ether 88:00:33:77:b0:b0
ifconfig wlan0 up
END
~~~

все
