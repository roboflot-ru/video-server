#pragma once

#include "Buffer.h"

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"
#include "ServerMediaSession.hh"

#include <map>
#include <string>
#include <thread>

class Live
{
public:
  Live();
  ~Live();

  void createSession(Buffer& payloadBuffer, unsigned port, const std::string& uid);
  void removeSession(unsigned port, const std::string& uid);

private:
  RTSPServer* getRtspServer(unsigned port);

private:
  TaskScheduler* Scheduler;
  UsageEnvironment* Env;
  std::map<unsigned, RTSPServer*> Servers;
};
