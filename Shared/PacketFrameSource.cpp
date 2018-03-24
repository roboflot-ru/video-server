#include "PacketFrameSource.h"
#include "InputFile.hh"
#include "GroupsockHelper.hh"
#include <iostream>

extern "C" {
#include <unistd.h> //sleep
}

PacketFrameSource::PacketFrameSource(UsageEnvironment& env, Buffer& payloadBuffer)
  : FramedSource(env)
  , PayloadBuffer(payloadBuffer)
{
}

PacketFrameSource::~PacketFrameSource()
{
}

void PacketFrameSource::doGetNextFrame()
{
  while (!PayloadBuffer.GetDataSize())
  {
    usleep(10);
  }
  fFrameSize = std::min(fMaxSize, PayloadBuffer.GetDataSize());
  memcpy(fTo, PayloadBuffer.GetData(), fFrameSize);
  PayloadBuffer.Consolidate(fFrameSize);

  gettimeofday(&fPresentationTime, NULL);
  nextTask() = envir().taskScheduler().scheduleDelayedTask(0,
    (TaskFunc*)FramedSource::afterGetting, this);
}

void PacketFrameSource::doStopGettingFrames() 
{
  envir().taskScheduler().unscheduleDelayedTask(nextTask());
}

