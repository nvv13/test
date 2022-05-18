#!/bin/bash

cd /home/nvv/gitflic.ru/test

git add .
git commit -m "sync1"
git push


rsync -r --links --exclude=.git --delete /home/nvv/gitflic.ru/test /home/nvv/github.com

cd /home/nvv/github.com/test

git add .
git commit -m "sync2"
git push


