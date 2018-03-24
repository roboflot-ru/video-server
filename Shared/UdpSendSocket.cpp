#include "UdpSendSocket.h"

#include "string.h"
#include <stdexcept>
#include <string>

extern "C" {
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h> //sleep
#include <netdb.h> 
}

UdpSendSocket::UdpSendSocket(const std::string& host, unsigned port)
  : Port(port)
  , Host(host)
  , SendSocket(-1)
{
}

UdpSendSocket::~UdpSendSocket()
{
  if (SendSocket > 0)
  {
    close(SendSocket);
  }
}

void UdpSendSocket::Connect()
{
  if (SendSocket == -1)
  {
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  }

  if (SendSocket == -1)
  {
    throw std::runtime_error("failed to create incoming UDP socket\n");
  }

  // zero out the structure
  memset((char *)&si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(Port);
  if (inet_aton(Host.c_str(), &si_me.sin_addr) == 0)
  {
    throw std::runtime_error("failed inet_aton\n");
  }
}

unsigned UdpSendSocket::Send(const unsigned char* data, unsigned size)
{
  while (true)
  {
    ssize_t count = sendto(SendSocket, data, size, 0, (sockaddr*)&si_me, sizeof(si_me));
    if (count < 0)
    {
      if (errno == EAGAIN)
      {
        Connect();
      }
      continue;
    }
    return count;
  }

  return 0;
}

std::string UdpSendSocket::GetHost() const
{
  return Host;
}

unsigned UdpSendSocket::GetPort() const
{
  return Port;
}