INC=-I../Shared -I/opt/vc/include -I/usr/include -I../restbed/source -I../restbed/dependency/asio/asio/include -I.. -I/usr/local/include/liveMedia -I/usr/local/include/groupsock -I/usr/local/include/UsageEnvironment -I/usr/local/include/BasicUsageEnvironment
PRELIB= ../restbed/build/librestbed.a -L/opt/vc/lib -lpthread -lliveMedia -lBasicUsageEnvironment -lgroupsock -lUsageEnvironment -lssl -lcrypto
CPPFLAGS = -std=c++11
BUILDLIST=LiveReciever.cpp main.cpp WebServer.cpp
SHAREDBUILDLIST=../Shared/Buffer.cpp ../Shared/Config.cpp ../Shared/File.cpp ../Shared/H264Subsession.cpp ../Shared/Live.cpp ../Shared/LockedBuffer.cpp ../Shared/MutexHolder.cpp ../Shared/PacketFrameSource.cpp ../Shared/UdpListenSocket.cpp 
THIRDPARTYBUILDLIST=../ini/cpp/INIReader.cpp ../ini/ini.c

all:
	g++ $(CPPFLAGS) $(INC) $(BUILDLIST) $(SHAREDBUILDLIST) $(THIRDPARTYBUILDLIST) $(PRELIB) -o VideoService -ldl -lrt

	
clean:
	rm -rf VideoService *.o
	rm -r *.h264
