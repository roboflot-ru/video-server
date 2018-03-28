#include "Live.h"
#include "H264Subsession.h"
#include "TimeUtils.h"

#include <iostream>
#include <vector>

Live::Live()
{
  Scheduler = BasicTaskScheduler::createNew();
  Env = BasicUsageEnvironment::createNew(*Scheduler);
}

RTSPServer* Live::getRtspServer(unsigned port)
{
  if (Servers.count(port))
  {
    return Servers[port];
  }

  RTSPServer* rtspServer = RTSPServer::createNew(*Env, port, NULL);
  if (rtspServer == NULL)
  {
    throw std::runtime_error(std::string("Failed to create RTSP server: ") + Env->getResultMsg());
  }

  if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080))
  {
  }

  auto eventLoop = [this]()
  {
    try
    {
      Env->taskScheduler().doEventLoop();
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << std::endl;
    }
  };
  std::thread(eventLoop).detach();
  
  Servers[port] = rtspServer;
  return rtspServer;
}

void Live::createSession(Buffer& payloadBuffer, unsigned port, const std::string& uid)
{
  H264Subsession *h264Subsession = new H264Subsession(*Env, payloadBuffer);
  ServerMediaSession* sms = ServerMediaSession::createNew(*Env, uid.c_str(), uid.c_str(), "Drone live stream session");
  sms->addSubsession(h264Subsession);
  getRtspServer(port)->addServerMediaSession(sms);
}

void Live::removeSession(unsigned port, const std::string& uid)
{
  getRtspServer(port)->removeServerMediaSession(uid.c_str());
}

Live::~Live()
{
}


