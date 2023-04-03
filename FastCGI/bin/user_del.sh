#!/bin/bash

FILEINFO="./infotext.text"

sed -i `grep -n -w "${1}" $FILEINFO | awk -F : '{print $1}'`d $FILEINFO