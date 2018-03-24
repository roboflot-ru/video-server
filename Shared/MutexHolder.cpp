#include "MutexHolder.h"

MutexHolder::MutexHolder(std::mutex& mutex)
  : Mutex(mutex)
{
  Mutex.lock();
}

MutexHolder::~MutexHolder()
{
  Mutex.unlock();
}

TryMutexHolder::TryMutexHolder(std::mutex& mutex)
  : Mutex(mutex)
{
  Mutex.try_lock();
}

TryMutexHolder::~TryMutexHolder()
{
  Mutex.unlock();
}
