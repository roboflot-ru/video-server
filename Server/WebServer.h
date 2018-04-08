#pragma once

#include "Live.h"
#include "LiveReciever.h"

#include <map>
#include <memory>
#include <restbed>

class WebServer
{
public:
  WebServer(int port, const std::string& noSignalPath);
  void Run();

private:
  void GetRegister(const std::shared_ptr<restbed::Session> session);
  void GetUnregister(const std::shared_ptr<restbed::Session> session);
  void GetStatus(const std::shared_ptr<restbed::Session> session);
  void CheckLocalRequest(const std::shared_ptr<restbed::Session> session);

private:
  int Port;
  const std::string NoSignalPath;
  restbed::Service WebService;
  std::map<std::string, std::shared_ptr<LiveReciever> > Recievers;
  Live RtspLive;
};
