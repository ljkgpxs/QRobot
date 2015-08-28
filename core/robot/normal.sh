#! /bin/bash
# QRobot Project By Ljkgpxs

cd $robot_dir

qfile="./ques.txt"
afile="./ans.txt"


# 检查是否有脏话 使用模糊搜索
./same "$@" ./err.lst
if [ $? != 0 ]
then
	echo "骂人的孩纸不是好好孩纸~"
	exit 0
fi

# 搜索问题 使用绝对搜索 绝对数据由用户添加
./same_abs "$@" ./ques_abs.txt
ret=$?
if [ $ret != 0 ]
then
	echo "`./findans $ret ./ans_abs.txt`"
	exit 0;
fi

# 搜索问题 使用模糊搜索，模糊数据由管理员管理
./same "$@" $qfile
ans="`./findans $? $afile `"

if [ $? == 0 ]
then
	echo "妹抖不理解您的意思,可以教我吗?"
	echo "教我的方法可以问我哦!"
else
	if [ ${ans:0:1} == '#' ]
		then
		echo ${ans:1} >/tmp/"$uid"_bashqq.cmd
		chmod 775 /tmp/"$uid"_bashqq.cmd
		bash /tmp/"$uid"_bashqq.cmd
		rm /tmp/"$uid"_bashqq.cmd
	else
		echo "$ans"
	fi
fi

