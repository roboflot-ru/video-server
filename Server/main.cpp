#include "Config.h"
#include "WebServer.h"

#include <iostream>

int main(int argc, char **argv) 
{
  try
  {
    Config config;

    int httpServerPort = config.GetInt("common", "http_server_port");
    
    WebServer webServer(httpServerPort);
    webServer.Run();
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
