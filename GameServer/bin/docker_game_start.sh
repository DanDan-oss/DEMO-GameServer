#!/bin/bash

#启动游戏守护进程
./game.out

while [ a = a ]
do
	sleep 10
	# v 取反的意思 -w全字匹配
	ps aux | grep -w ./game.out | grep -v grep
	if [ 0 -ne $? ]
	then
		# 检测到进程退出,应该讲Redis中的记录容器的信息删除
		
		exit 0
	fi
done
