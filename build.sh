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

