#pragma once

#include "Live.h"
#include "LockedBuffer.h"
#include "Timer.h"
#include "UdpListenSocket.h"

#include <map>
#include <mutex> 
#include <thread>
#include <vector>

class LiveReciever
{
public:
  LiveReciever(Live& rtspLive, unsigned portIn, unsigned portOut, const std::string& uid);
  ~LiveReciever();

  void operator()();

private:
  void ReceiveThreadFunc();
  void Receive();
  void SendIfPossible();
  void WriteNextPacket(unsigned char* data, unsigned size);
  void StoreHeader(const unsigned char* data, unsigned size);

private:
  bool Canceled;
  bool Started;
  LockedBuffer PayloadBuffer;
  Live& RtspLive;
  UdpListenSocket ListenSocket;
  std::thread ReceiveThread;
  std::vector<unsigned char> Header;
  std::map<unsigned, std::vector<unsigned char> > SequencedRtp;
  unsigned PrevPacketNumber;
  unsigned PortOut;
  const std::string Uid;
  Timer<LiveReciever> NoSignalTimer;
  bool NoSignal;
  std::vector<unsigned char> NoSignalData;
  std::mutex Mutex;
};
