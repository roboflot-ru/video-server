#include "TimeUtils.h"

#include <iostream>
#include <sys/time.h>

unsigned GetTicks()
{
    struct timeval tv;
    if (gettimeofday(&tv, 0))
        return 0;
    return (tv.tv_usec/1000+tv.tv_sec*1000);
}

std::string GetTime()
{
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,80,"%d-%m-%Y %H-%M-%S",timeinfo);
  return std::string(buffer);
}

Profile::Profile(const std::string& name)
  : Name(name)
  , Time(GetTicks())
{
}

Profile::~Profile()
{
  std::cout << Name << " " << (GetTicks() - Time) << std::endl;
}
