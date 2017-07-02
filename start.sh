#forced killing another gzserver instance
pkill -9 gzserver
pkill roscore -9
pkill rosmaster -9
./gzweb/stop_gzweb

# add kinetic to source
source /opt/ros/kinetic/setup.bash
address=$(ifconfig | grep '(?<=inet addr:)\d{1,3}[.]\d{1,3}[.]\d{1,3}[.]\d{1,3}' -P -m 1 -o)

#use last version of node.js
source ~/.nvm/nvm.sh
nvm use "8.1.2"
echo nvm -v

#start gazebo server
gnome-terminal -x sh -c "
 roslaunch rossubscriber Main.Launch; exec bash"

#start gzweb
gnome-terminal -x sh -c "../gzweb/start_gzweb.sh; exec bash"

#start gzbridge
gnome-terminal -x sh -c "./start_bridge.sh $address; exec bash"
