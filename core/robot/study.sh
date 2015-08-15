#! /bin/bash

cd $robot_dir

ques="`./getdata "$@" 1`"
ans="`./getdata "$@" 2`"
if [ -z $ques ] || [ -z $ans ]
then
	echo "不要填写空白哦!"
	exit 0
fi

./same "$ques" ./err.lst
errq=`echo $?`
./same "$ans" ./err.lst
erra=`echo $?`
if [ $errq == 0 ] && [ $erra == 0 ]
then
	:
else
	echo "不允许骂人哦,妹抖是会学坏的说!"
	exit 0
fi


if [ `grep $uid ./allow.lst | wc -l` != 0 ]
then
	./same "$ques" ./ques.txt
	errq=`echo $?`
	if [ $errq  == 0 ]
	then
		echo "$ques" >>./ques.txt
		echo "$ans" >>./ans.txt
		echo "妹抖记住啦!!!我的主人!"
		echo "问:$ques,答:$ans"
	else
		echo "妹抖已经会了,不用再教这个了!"
	fi
else
	./same_abs "$ques" ./ques_abs.txt
	errq=`echo $?`
	if [ $errq  == 0 ]
	then
		echo "$ques" >>./ques_abs.txt
		echo "$ans" >>./ans_abs.txt
		echo "妹抖记住啦!"
		echo "问:$ques,答:$ans"
	else
		echo "妹抖已经会了,不用再教这个了!"
	fi
fi
exit 0

