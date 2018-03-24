#pragma once

#include <memory>

class Buffer
{
public:
  Buffer(unsigned size, const unsigned char* data = 0);
  virtual void Add(const unsigned char* data, unsigned size);
  virtual void Reset();
  virtual unsigned char* GetData();
  virtual unsigned GetDataSize();
  virtual unsigned GetSize();
  virtual void Consolidate(unsigned offset);

private:
  void Resize(unsigned newSize);

private:
  std::unique_ptr<unsigned char> Data;
  unsigned Size;
  unsigned Offset;
};