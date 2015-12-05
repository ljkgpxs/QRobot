#/bin/bash
# QRobot Project By Ljkgpxs

# $uid is fromer QQ number
# $context is message content

# $1 message type; $2 Fromer QQ number; $3 message content
#cd ./core/
###################
. ./conf/env

if [ -f ./qqlog.txt ]; then
	:
else
	touch ./qqlog.txt
fi

export uid=$2
export context=$3

same="$robot_dir/same"
errfile="$robot_dir/err.lst"
lognum=0
other=0
function check_err
{
#	echo "Start $PWD $1"
	$same "$1" $errfile
	ret=$?
#	echo "Return code $ret"
	if [ $ret != 0 ]
	then
		echo "不要说脏话哦！"
	fi
}


function func
{
	lognum=`grep $uid qqlog.txt | wc -l`
	special_list=`grep $uid ./conf/special_list.txt | wc -l`
	
	if [ "${context:0:1}" == "@" ] || [ "${context:0:1}" == "＠" ]; then
		if [ -z ${context:1} ]
		then
			echo "您要说什么呢？"
		else  
#			echo "${context:1}"
			case "${context:1}" in
				"<img "* )
					emohandle "${context:1}";;
				"http:*" )
					:;;
				'#'* )
#					echo "get command"
					if [ `grep $uid $robot_dir/allow.lst | wc -l` == 0 ]; then
						echo "Permission denied"
					else
						echo "${context:2}" >/tmp/robot_shell.sh
						chmod 777 /tmp/robot_shell.sh
						/tmp/robot_shell.sh
						ret=$?
						rm /tmp/robot_shell.sh
						if [ $ret == 0 ]; then
							echo "Successful"
						else
							echo "Failed with $ret"
						fi
					fi
					;;
				* )
#					echo "Got Normal Message"
					$robot_dir/normal.sh "${context:1}";;
			esac
		fi
		
	else
#		echo "Push to robot"
		case "$context" in
		\?*-* |  ？*-*)
			$robot_dir/study.sh "$context";;
		* )
#			echo lognum  $lognum special_list $special_list
		       if [ $lognum == 0 ] && [ $special_list == 0 ]
		       then	
			       echo "您好，我是机器人-妹抖.
			如果想和我聊天，可以在消息前面加上@哦!"
				check_err "$context"
			else 
				check_err "$context"
				if [ $lognum == 5 ]
			then
				echo "主人真的不在啦，妹抖从来都不骗人的说，不介意的话就和我聊天嘛!
				消息前面不要忘了加@哦！"
			fi
			fi
			echo $uid >>./qqlog.txt;;
		esac
	fi
	exit 0
}

func
