читаем

01_setup_ESP-IDF.txt  

02_new_prj.txt

;


делаем перед сборкой и т.д.
$ source ~/esp-idf/export.sh


или собирать все с помощью docker
например idf.py build

$ docker run --rm -v $PWD:/project -w /project -u $UID -e HOME=/tmp espressif/idf idf.py build

PS:
 https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32/api-guides/tools/idf-docker-image.html

~~~
еще можно Using the image interactively!!!
It is also possible to do builds interactively, to debug build issues or test the automated build scripts. Start the container with -i -t flags:

docker run --rm -v $PWD:/project -w /project -u $UID -e HOME=/tmp -it espressif/idf
Then inside the container, use idf.py as usual:

idf.py menuconfig
idf.py build
~~~