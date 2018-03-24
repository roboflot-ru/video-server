#include "Config.h"

#include <stdexcept>

Config::Config()
  : Reader("VideoService.ini")
{
  if (Reader.ParseError() < 0) 
  {
    throw std::runtime_error("Failed to parse config file VideoService.ini. Error - " + std::to_string(Reader.ParseError()));
  }
}

std::string Config::GetString(const std::string& section, const std::string& param) const
{
  std::string result = Reader.Get(section, param, "");
  if (result.empty())
  {
    throw std::runtime_error("Failed to get string value from config file. Section - " + section + " Param - " + param);
  }

  return result;
}

int Config::GetInt(const std::string& section, const std::string& param) const
{
  int result = Reader.GetInteger(section, param, -1);
  if (result == -1)
  {
    throw std::runtime_error("Failed to get int value from config file. Section - " + section + " Param - " + param);
  }

  return result;
}
