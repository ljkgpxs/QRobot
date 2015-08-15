#!/bin/bash
cd core
#./robot.sh $1 $2 $3
reply_content=`./robot.sh $1 $2 $3`
printf "$reply_content"  >/tmp/"$2".reply

