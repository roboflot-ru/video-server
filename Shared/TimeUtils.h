#pragma once

#include <string>

unsigned GetTicks();
std::string GetTime();

class Profile
{
public:
  Profile(const std::string& name);
  ~Profile();

private:
  const std::string Name;
  const unsigned Time;
};