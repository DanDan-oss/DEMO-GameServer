#!/bin/bash

CONTAINERID=$1

# 停止运行中的容器
sudo docker stop $CONTAINERID 2>& 1> /dev/null
# 删除容器
sudo docker rm $CONTAINERID 2>& 1> /dev/null