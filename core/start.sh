#!/bin/bash
# QRobot Project By Ljkgpxs
cd core
#./robot.sh $1 $2 $3
reply_content=`./robot.sh $1 $2 $3`
echo -n $reply_content  >/tmp/"$2".reply
