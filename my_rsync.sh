#!/bin/bash

rsync -vr --exclude=.git --delete /home/nvv/gitflic.ru/test /home/nvv/github.com

cd /home/nvv/github.com/test

git add .
git commit -m "sync"
git push


