#pragma once

#include "Buffer.h"
#include "H264VideoFileServerMediaSubsession.hh"

class H264Subsession : public H264VideoFileServerMediaSubsession
{
public:
  H264Subsession(UsageEnvironment& env, Buffer& payloadBuffer);

  virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);

private:
  Buffer& PayloadBuffer;
};
