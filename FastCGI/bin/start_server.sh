#!/bin/bash
  
APPLICATION="-f $1"
SERVERPORT="-p $2"
SERVERADDR="-a 127.0.0.1"

spawn-fcgi $SERVERADDR  $SERVERPORT $APPLICATION