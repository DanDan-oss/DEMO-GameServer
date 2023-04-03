#!/bin/bash

IMAGEBASE=game:v1.0
IMAGEAPP=docker_game_start

sudo docker run --rm  -e ROOM_INFO=$1 -d  -P $IMAGEBASE 2>& 1> /dev/null
# 过滤查询最新创建的那1个[-n 1]
sudo docker ps -n 1 -f ancestor=$IMAGEBASE | grep $IMAGEAPP | awk -F "->" '{print $1}' | awk -F : '{print $NF}'
