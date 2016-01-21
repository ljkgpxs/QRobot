#!/bin/sh
# QRobot Project By Ljkgpxs

if [ -f `which gcc` ]; then
	:
else
	echo "Gcc not found, please install Gcc,or compile core/robot/*.c by yourself."
	exit 1;
fi

cd core/
echo "**Building Robot"
./build.sh
echo "**Done"

cd ..
echo "please input tuling robot api key, if you dont have it, input ENTER directly(http://www.tuling123.com)"
read key
if [ $key ]; then
	echo "Writting key to \"core/robot-tuling/settings.cfg\""
	echo $key >core/robot-tuling/settings.cfg
else
	echo "No key input...skip"
	print >core/robot-tuling/sttings.cfg
fi


