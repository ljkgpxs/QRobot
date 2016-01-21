#!/bin/sh
. ./conf/env
cd $robot_dir

echo "	CC findans"
gcc findans.c -o findans
chmod 775 findans

echo "	CC same"
gcc same.c -o same
chmod 775 same

echo "	CC getdata"
gcc getdata.c -o getdata
chmod 775 getdata

echo "	CC same_abs"
gcc same_abs.c -o same_abs
chmod 775 same_abs

cd ../robot-tuling

echo "	CC tuling-cli(optional)"
gcc tuling-cli.c cJSON.c -o test -lcurl -lm
if [ $? != 0 ]; then
	echo "Please install libcurl"
fi

