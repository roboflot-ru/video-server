#pragma once

class UdpListenSocket
{
public:
  UdpListenSocket(unsigned port);
  ~UdpListenSocket();

  void Connect();
  unsigned Listen(unsigned char* data, unsigned size);

private:
  unsigned Port;
  int ListenSocket;
};