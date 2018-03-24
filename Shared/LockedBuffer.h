#pragma once

#include "Buffer.h"
#include <mutex> 

class LockedBuffer : public Buffer
{
public:
  LockedBuffer(unsigned size, const unsigned char* data = 0);
  virtual void Add(const unsigned char* data, unsigned size);
  virtual void Reset();
  virtual unsigned char* GetData();
  virtual unsigned GetDataSize();
  virtual unsigned GetSize();
  virtual void Consolidate(unsigned offset);

private:
  std::mutex Mutex;
};