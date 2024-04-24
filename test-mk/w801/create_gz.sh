#!/bin/bash

rm -f ~/Nextcloud/mk/W801_SDK-linux.tar.gz
rm -f ~/Nextcloud/mk/W801_sdk-prj.tar.gz

tar cvzf ~/Nextcloud/mk/W801_SDK-linux.tar.gz ./W801_SDK-linux
tar cvzf ~/Nextcloud/mk/W801_sdk-prj.tar.gz ./sdk-prj

echo "файлики будут лежать по ссылке достоупной из интернета https://orangepi.nvv13.keenetic.link/nextcloud/index.php/s/6ZRAwg7taaC6Ho4"

