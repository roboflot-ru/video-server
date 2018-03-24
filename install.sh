#!/bin/bash

sudo apt-get install cmake libssl-dev libopencv-dev curl

rm -rf restbed/
git clone --recursive https://github.com/corvusoft/restbed.git
mkdir restbed/build
cd restbed/build
sudo cmake [-DBUILD_TESTS=YES] [-DBUILD_EXAMPLES=YES] [-DBUILD_SSL=NO] [-DBUILD_SHARED=YES] [-DCMAKE_INSTALL_PREFIX=/output-directory] ..
sudo make
sudo make install
sudo ldconfig
make test
cd ../..

wget http://www.live555.com/liveMedia/public/live555-latest.tar.gz
tar -xf live555-latest.tar.gz
cd live
./genMakefiles linux
make
sudo make install
sudo ldconfig
