#pragma once

#include "Buffer.h"
#include "FramedFileSource.hh"

class PacketFrameSource : public FramedSource
{
public:
  PacketFrameSource(UsageEnvironment& env, Buffer& payloadBuffer);

  virtual ~PacketFrameSource();

private:
  virtual void doGetNextFrame();
  virtual void doStopGettingFrames();

private:
  Buffer& PayloadBuffer;
};