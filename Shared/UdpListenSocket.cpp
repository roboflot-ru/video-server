#include "UdpListenSocket.h"

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

#include <iostream>

UdpListenSocket::UdpListenSocket(unsigned port)
  : Port(port)
  , ListenSocket(-1)
{
}

UdpListenSocket::~UdpListenSocket()
{
  if (ListenSocket > 0)
  {
    close(ListenSocket);
  }
}

void UdpListenSocket::Connect()
{
  if (ListenSocket == -1)
  {
    ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  }

  if (ListenSocket == -1)
  {
    throw std::runtime_error("failed to create incoming UDP socket " + std::to_string(errno) + " " + strerror(errno) + "\n");
  }

  struct sockaddr_in si_me;
  memset((char *)&si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(Port);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(ListenSocket, (sockaddr*)&si_me, sizeof(si_me)) == -1)
  {
    throw std::runtime_error("failed to bind socket " + std::to_string(errno) + " " + strerror(errno) + "\n");
  }
}

unsigned UdpListenSocket::Listen(unsigned char* data, unsigned size)
{
  struct sockaddr_storage src_addr;
  socklen_t src_addr_len = sizeof(src_addr);

  while (true)
  {
    ssize_t count = recvfrom(ListenSocket, data, size, 0, (struct sockaddr*)&src_addr, &src_addr_len);
    if (count < 0)
    {
      std::cout << "received zero buffer " << std::to_string(errno) << " " << strerror(errno) << std::endl;
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