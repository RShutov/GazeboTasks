#kill existing gzserver
pkill gzserver

cd build
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:~/GazeboTask/build
gazebo --verbose ../source/velodyne.world
cd ..
