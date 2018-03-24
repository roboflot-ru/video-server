#pragma once

#include "LiveReciever.h"

#include <map>
#include <memory>
#include <restbed>

class WebServer
{
public:
  WebServer(int port);
  void Run();

private:
  void GetRegister(const std::shared_ptr<restbed::Session> session);
  void GetUnregister(const std::shared_ptr<restbed::Session> session);
  void GetStatus(const std::shared_ptr<restbed::Session> session);

private:
  int Port;
  restbed::Service WebService;
  std::map<std::string, std::shared_ptr<LiveReciever> > Recievers;
};
