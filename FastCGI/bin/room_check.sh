#!/bin/bash
IMAGEBASE=game:v1.0
IMAGEAPP=docker_game_start
IMAGEPORT=$1


if [ "$IMAGEPORT" == "" ]
then
    # 查询容器ID
    #sudo docker ps -f ancestor=$IMAGEBASE | grep $IMAGEAPP | awk -F $IMAGEBASE '{print $1}'
    # 查询所有容器对应映射的端口号
    sudo docker ps -f ancestor=$IMAGEBASE | grep $IMAGEAPP | awk -F "->" '{print $1}' | awk -F : '{print $NF}'
else 
    sudo docker ps -f ancestor=$IMAGEBASE | grep $IMAGEAPP | awk -F "->" '{print $1}' | awk -F : '{print $NF}' | grep "$IMAGEPORT" 2>$ 1> /dev/null
fi