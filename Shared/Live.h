#pragma once

#include "Buffer.h"

#include <string>
#include <thread>

class Live
{
public:
  Live(Buffer& payloadBuffer, unsigned port, const std::string& uid);
  ~Live();

private:
  Buffer& PayloadBuffer;
  unsigned Port;
  std::thread RtspServerThread;
};
