#!/bin/bash

USERNAME=$1
PASSWORD=$2
FILEINFO="./infotext.text"

# 可以使用 echo $? 查看返回值 0代表查询正确 >=1 代表查询失败
# 讲fd2和 fd1 重定位到/dev/null

if [ "$PASSWORD" == "" ]
then
	cat $FILEINFO | grep -w $USERNAME 2>& 1> /dev/null
else 
	cat $FILEINFO | grep -w "$USERNAME:$PASSWORD" 2>$ 1> /dev/null
fi
