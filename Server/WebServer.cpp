#include "WebServer.h"

#include <functional>
#include <iostream>

WebServer::WebServer(int port, const std::string& noSignalPath)
  : Port(port)
  , NoSignalPath(noSignalPath)
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
    CheckLocalRequest(session);
    auto request = session->get_request();
    int portIn = stoi(request->get_query_parameter("port_in"));
    int portOut = stoi(request->get_query_parameter("port_out"));
    std::string uid = request->get_query_parameter("uid");
    Recievers[uid] = std::make_shared<LiveReciever>(RtspLive, portIn, portOut, uid, NoSignalPath);
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
  CheckLocalRequest(session);
  auto request = session->get_request();
  std::string uid = request->get_query_parameter("uid");
  Recievers.erase(uid);
  session->close(200, "{status: \"ok\"}\n");
}

void WebServer::GetStatus(const std::shared_ptr<restbed::Session> session)
{
  CheckLocalRequest(session);
  session->close(200, "{status: \"ok\"}\n");
}

void WebServer::CheckLocalRequest(const std::shared_ptr<restbed::Session> session)
{
  auto origin = session->get_origin();
  auto prefix = origin.find("::ffff:");
  auto bracket = origin.find("]");
  if (prefix != std::string::npos && bracket != std::string::npos)
  {
    auto start = prefix + sizeof("::ffff:") - 1;
    auto ip = origin.substr(start, bracket - start);
    if (ip == "localhost" || ip == "127.0.0.1")
    {
      return;
    }
    std::cout << "ip " << ip << std::endl;
  }

  if (origin.find("::1") != std::string::npos)
  {
    return;
  }

  std::cout << "Allowed only local requests " << origin << std::endl;
  throw std::runtime_error("Allowed only local requests");
}

void WebServer::Run()
{
  auto settings = std::make_shared<restbed::Settings>();
  settings->set_port(Port);
  settings->set_default_header("Connection", "close");
  std::cout << "Listen http requests on port " << Port << std::endl;
  WebService.start(settings);
}
