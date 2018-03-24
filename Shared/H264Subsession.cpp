#include "H264Subsession.h"
#include "PacketFrameSource.h"

#include "H264VideoStreamFramer.hh"

#include <iostream>

H264Subsession::H264Subsession(UsageEnvironment& env, Buffer& payloadBuffer)
  : H264VideoFileServerMediaSubsession(env, "", True)
  , PayloadBuffer(payloadBuffer)
{
}

FramedSource* H264Subsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{
  estBitrate = 1000000;
  auto h264 = new PacketFrameSource(envir(), PayloadBuffer);
  return H264VideoStreamFramer::createNew(envir(), h264);
}
