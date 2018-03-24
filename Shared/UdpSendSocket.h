#pragma once

#include <string> 

extern "C" {
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h> //sleep
#include <netdb.h> 
}

class UdpSendSocket
{
public:
  UdpSendSocket(const std::string& host, unsigned port);
  ~UdpSendSocket();

  void Connect();
  unsigned Send(const unsigned char* data, unsigned size);

  std::string GetHost() const;
  unsigned GetPort() const;

private:
  const std::string Host;
  const unsigned Port;
  int SendSocket;
  sockaddr_in si_me;
};