#pragma once

#include <condition_variable>
#include <mutex> 

class Event
{
public:
  Event(bool signaled = false)
    : Signaled(signaled)
  {
  }
  
  void Set()
  {
    Signaled = true;
    ConditionVariable.notify_all();
  }

  void Reset()
  {
    Signaled = false;
  }

  bool Wait(unsigned milliseconds = 0)
  {
    if (!Signaled)
    {
      std::unique_lock<std::mutex> lock(ConditionMutex);
      if (milliseconds)
      {
        return (ConditionVariable.wait_for(lock, std::chrono::milliseconds(milliseconds)) == std::cv_status::timeout);
      }
      else
      {
        ConditionVariable.wait(lock);
        return true;
      }
    }
  }

private:
  std::mutex ConditionMutex;
  std::condition_variable ConditionVariable;
  bool Signaled;
};

typedef std::shared_ptr<Event> EventPtr;