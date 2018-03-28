#include "LiveReciever.h"
#include "KeyFrame.h"

#include <iostream>

// 3 sec for 1MB bitrate
const unsigned MaxPayloadBufferSize = 3*128*1024;

LiveReciever::LiveReciever(Live& rtspLive, unsigned portIn, unsigned portOut, const std::string& uid)
  : Canceled(false)
  , Started(false)
  , PayloadBuffer(MaxPayloadBufferSize)
  , RtspLive(rtspLive)
  , ListenSocket(portIn)
  , PrevPacketNumber(0)
  , PortOut(portOut)
  , Uid(uid)
{
  RtspLive.createSession(PayloadBuffer, portOut, uid);
  ListenSocket.Connect();

  ReceiveThread = std::thread(&LiveReciever::ReceiveThreadFunc, this);
}

LiveReciever::~LiveReciever()
{
  Canceled = true;
  RtspLive.removeSession(PortOut, Uid);
  if (ReceiveThread.joinable())
    ReceiveThread.join();
}

void LiveReciever::ReceiveThreadFunc()
{
  try
  {
    Receive();
  }
  catch (const std::exception& e)
  {
    std::cout << "Error receive incoming stream " << e.what() << std::endl;
  }
}

void LiveReciever::Receive()
{
  unsigned char buffer[256 * 1024];
  while (!Canceled)
  {
    auto size = ListenSocket.Listen(&buffer[0], sizeof(buffer), Canceled);
    if (size <= 0)
    {
      continue;
    }
    unsigned packetNumber = 0;
    memcpy(&packetNumber, buffer, sizeof(packetNumber));
    auto packetSize = size - sizeof(packetNumber);
    if (packetNumber == 0)
    {
      StoreHeader(buffer + sizeof(packetNumber), packetSize);
      continue;
    }

    std::vector<unsigned char> payload(packetSize);
    memcpy(&payload[0], buffer + sizeof(packetNumber), packetSize);
    SequencedRtp[packetNumber] = payload;

    SendIfPossible();
  }
}

void LiveReciever::SendIfPossible() 
{
  while (!SequencedRtp.empty())
  {
    if (PrevPacketNumber == 0)
    {
      PrevPacketNumber = SequencedRtp.begin()->first;
    }

    unsigned short packetNumber = PrevPacketNumber + 1;
    if (SequencedRtp.count(packetNumber))
    {
      auto& data = SequencedRtp[packetNumber];
      WriteNextPacket(&data[0], data.size());
      PrevPacketNumber = packetNumber;
      SequencedRtp.erase(packetNumber);
      continue;
    }

    if (SequencedRtp.size() > 25)
    {
      std::cout << "Cannot determine rtp sequence. Skip packet " << packetNumber << std::endl;
      ++PrevPacketNumber;
      continue;
    }
    break;
  }
}

void LiveReciever::WriteNextPacket(unsigned char* data, unsigned size)
{
  if (isKeyFrame(data))
  {
    if (PayloadBuffer.GetDataSize() > MaxPayloadBufferSize)
    {
      PayloadBuffer.Reset();
    }
    PayloadBuffer.Add(&Header[0], Header.size());
  }
  else if (!Started)
  {
    return;
  }

  Started = true;
  PayloadBuffer.Add(data, size);
}


void LiveReciever::StoreHeader(const unsigned char* data, unsigned size)
{
  Header.resize(size);
  memcpy(&Header[0], data, size);
}
