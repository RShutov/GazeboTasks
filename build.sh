if ! [-d "build"]
then
	mkdir build
fi
cd build
pwd
cmake ..
make
