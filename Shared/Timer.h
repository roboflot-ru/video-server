#pragma once

#include <thread>
#include <tr1/functional>

#include "Event.h"

template<typename ActionType>
class Timer
{
public:
  Timer(ActionType& action, unsigned timeout, bool repeat = true)
    : Action(action)
    , Timeout(timeout)
    , Repeat(repeat)
    , Cancel(false)
  {
    Thread = std::thread(std::tr1::bind(&Timer::ThreadFunc, this));
  }

  ~Timer()
  {
    Cancel = true;
    Reset();
    Thread.join();
  }
  
  void ThreadFunc()
  {
    do
    {
      TimerEvent.Reset();
      if (TimerEvent.Wait(Timeout))
      {
        Action();
      }
    } while (Repeat && !Cancel);
  }

  void Reset()
  {
    TimerEvent.Set();
  }
  
private:
  ActionType& Action;
  unsigned Timeout;
  bool Repeat;
  bool Cancel;
  std::thread Thread;
  Event TimerEvent;
};