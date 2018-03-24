#include "WebServer.h"

#include <functional>
#include <iostream>

WebServer::WebServer(int port)
  : Port(port)
{
  auto registerResource = std::make_shared<restbed::Resource>();
  registerResource->set_path("/register");
  registerResource->set_method_handler("GET", std::bind(&WebServer::GetRegister, this, std::placeholders::_1));
  WebService.publish(registerResource);

  auto unregisterResource = std::make_shared<restbed::Resource>();
  unregisterResource->set_path("/unregister");
  unregisterResource->set_method_handler("GET", std::bind(&WebServer::GetUnregister, this, std::placeholders::_1));
  WebService.publish(unregisterResource);

  auto statusResource = std::make_shared<restbed::Resource>();
  statusResource->set_path("/status");
  statusResource->set_method_handler("GET", std::bind(&WebServer::GetStatus, this, std::placeholders::_1));
  WebService.publish(statusResource);
}

void WebServer::GetRegister(const std::shared_ptr<restbed::Session> session)
{
  try
  {
    auto request = session->get_request();
    int portIn = stoi(request->get_query_parameter("port_in"));
    int portOut = stoi(request->get_query_parameter("port_out"));
    std::string uid = request->get_query_parameter("uid");
    Recievers[uid] = std::make_shared<LiveReciever>(portIn, portOut, uid);
    session->close(200, "{status: \"ok\"}\n");
  }
  catch (const std::exception& e)
  {
    std::string errorString = std::string("{status: \"") + e.what() + std::string("\"}\n");
    session->close(500, errorString.c_str());
  }
}

void WebServer::GetUnregister(const std::shared_ptr<restbed::Session> session)
{
  auto request = session->get_request();
  std::string uid = request->get_query_parameter("uid");
  Recievers.erase(uid);
  session->close(200, "{status: \"ok\"}\n");
}

void WebServer::GetStatus(const std::shared_ptr<restbed::Session> session)
{
  session->close(200, "{status: \"ok\"}\n");
}

void WebServer::Run()
{
  auto settings = std::make_shared<restbed::Settings>();
  settings->set_port(Port);
  settings->set_default_header("Connection", "close");
  WebService.start(settings);
}
