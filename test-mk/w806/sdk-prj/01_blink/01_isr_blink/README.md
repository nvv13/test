
project task blink

board HLK-W806




Сборка  (это не конкретно этого проекта, а к примеру)
~~~
делал по инструкциям из этого проекта https://github.com/droppingy/W801_SDK-linux
   у меня Linux Fedora 33 x86_64, думаю для всех Linux x86_64 подойдет

1) сначала, один раз, ставим утилиты для сборки c-sky (архитектура w806, w801 мк)
   благодаря пользователю droppingy это очень просто!  

$ sudo git clone https://github.com/droppingy/hlk-w80x-toolchain.git /opt/w80x-tools

   так чтобы в итоге получилось /opt/w80x-tools/bin
   это чтоб в menuconfig не переделывать, такой там путь по умолчанию
   (еще, в этом мануале https://github.com/IOsetting/wm-sdk-w806, есть ссылки как получить данный Toolchain на другие архитектуры, типа i386)

2) далее качаем-распаковываем SDK

    перемещаемся в домашнюю директорию
 
$ cd ~

    делаем директорию для проекта

$ mkdir w806

$ cd w806  

    качаем

$ wget https://orangepi.nvv13.keenetic.link/nextcloud/index.php/s/twFmJCGsKNAdnz5/download/W806_SDK-linux.tar.gz

    распаковка

$ tar xvf W806_SDK-linux.tar.gz

3) далее, собираем приложение

$ cd W806_SDK-linux/

     если тут запустить make, то собирается демо приложение которое идет вместе с SDK, об этом я писал кратко в https://github.com/nvv13/test/tree/main/test-mk/w801 
     но мы хотим собрать другой проект
     для этого удаляем, лишние директории, 
      где app demo - директории с демо приложением
          bin - директория где в результате будет собранное приложение, создастся заново в процессе сборки

$ rm -R app demo bin

     делаем директорию для приложения

$ mkdir app

     и, в данном случае, качаем файлики приложения, 
     для примера, возьмем отсюда https://github.com/nvv13/test/tree/main/test-mk/w801/sdk-prj/02_task_blink
     но, принцип тот же самый и для других...(если нужно много файлов, то можно воспользоваться утилитой https://downgit.github.io/)

$ cd app

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/w806/sdk-prj/01_blink/01_blink_task/app/Makefile

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/w806/sdk-prj/01_blink/01_blink_task/app/main.c

     возвращаемся в директорию SDK
$ cd ..

     заменяем головной wget Makefile на наш (удалено обращение к директории demo)

$ rm Makefile

$ wget https://raw.githubusercontent.com/nvv13/test/main/test-mk/w806/sdk-prj/01_blink/01_blink_task/Makefile


     перед сборкой, очистить проект, иногда надо
$ make distclean


    сборка
$ make 


    млжно перед прошивкой, стереть флэш в микроконтроллере, иногда надо
$ make erase

    записать прошивку
$ make flash

  и тут програма вероятнее всего попросит нажать reset на платке для синхронизации

  нажимаем reset
   

    Всё!


~~~





Прочее:

  подцепляемся к платке, если интересует вывод на консоль (используеться в проекте)

$ picocom --echo -b 115200 /dev/ttyUSB0








