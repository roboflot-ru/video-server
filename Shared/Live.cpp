#include "Live.h"
#include "H264Subsession.h"
#include "TimeUtils.h"

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"
#include "ServerMediaSession.hh"

#include <iostream>
#include <vector>

Live::Live(Buffer& payloadBuffer, unsigned port, const std::string& uid)
  : PayloadBuffer(payloadBuffer)
  , Port(port)
{
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

  RTSPServer* rtspServer = RTSPServer::createNew(*env, Port, NULL);
  if (rtspServer == NULL)
  {
    throw std::runtime_error(std::string("Failed to create RTSP server: ") + env->getResultMsg());
  }

  H264Subsession *h264Subsession = new H264Subsession(*env, PayloadBuffer);
  ServerMediaSession* sms = ServerMediaSession::createNew(*env, uid.c_str(), uid.c_str(), "Drone live stream session");
  sms->addSubsession(h264Subsession);
  rtspServer->addServerMediaSession(sms);

  if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080))
  {
  }

  auto eventLoop = [env]()
  {
    try
    {
      env->taskScheduler().doEventLoop();
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << std::endl;
    }
  };
  RtspServerThread = std::thread(eventLoop);
}

Live::~Live()
{
  if (RtspServerThread.joinable())
    RtspServerThread.join();
}
