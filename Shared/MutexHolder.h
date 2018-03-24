#pragma once

#include <mutex> 

class MutexHolder
{
public:
  MutexHolder(std::mutex& mutex);
  ~MutexHolder();

private:
  std::mutex& Mutex;
};

class TryMutexHolder
{
public:
  TryMutexHolder(std::mutex& mutex);
  ~TryMutexHolder();

private:
  std::mutex& Mutex;
};