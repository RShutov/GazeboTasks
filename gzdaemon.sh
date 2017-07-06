#ROS daemon
usage()
{
	echo "$0 (start|stop)"
}

_log()
{
	process=$1
	shift
	echo "${process}[$$]:$*"
}

stop()
{
	pkill node -9
	pkill roscore -9
	pkill rosmaster -9
	pkill gzserver -9
	kill $(ps -ef | grep "(?<=shifouse)([ ]*)(\d+)(?=.+ros.+)" -P -o)
}

start()
{
	stop
	#try to kill instance of gazebo and ros

	#setting up path to catkin_ws
	source ~/catkin_ws/devel/setup.bash

	#setting up path to ros kinetic
	source /opt/ros/kinetic/setup.bash


	#setting up a hostanme and master uri
	ROS_HOSTNAME=104.236.122.114 
	ROS_MASTER_URI=http://104.236.122.114:22222
	
	#create log files
	touch ${LOG_DIR}/${LOG_GAZEBOROS}	
	touch ${LOG_DIR}/${LOG_GZWEB}
	touch ${LOG_DIR}/${LOG_GZBRIDGE}

	#run scripts
	nohup  start_gazebo_ros.sh  >> ${LOG_DIR}/${LOG_GAZEBOROS} &
	sleep 2
	nohup ../gzweb/start_gzweb.sh >> ${LOG_DIR}/${LOG_GZWEB} &
	sleep 2
	nohup start_bridge.sh $ROS_HOSTNAME >> ${LOG_DIR}/${LOG_GZBRIDGE} &
}

#setting Log directory
LOG_GAZEBOROS="gaseboros.log"
LOG_GZWEB="gzweb.log"
LOG_GZBRIDGE="gzbridge.log"
LOG_DIR="Log"
if [ ! -d "$LOG_DIR" ];then 
	mkdir $LOG_DIR
fi

case $1 in
	"start")
		start
		;;
	"stop")
		stop
		;;
	*)
		usage
		;;
esac
exit





