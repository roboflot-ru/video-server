#pragma once

#include "ini/cpp/INIReader.h"

#include <string>

class Config
{
public:
  Config();
  std::string GetString(const std::string& section, const std::string& param) const;
  int GetInt(const std::string& section, const std::string& param) const;

private:
  INIReader Reader;
};
