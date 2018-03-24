#include "Buffer.h"
#include <string.h>

Buffer::Buffer(unsigned size, const unsigned char* data)
  : Data(new unsigned char[size])
  , Size(size)
  , Offset(0)
{
  if (data)
  {
    memcpy(Data.get(), data, size);
    Offset = size;
  }
}

void Buffer::Add(const unsigned char* data, unsigned size)
{
  if (Offset + size > Size)
  {
    Resize((Offset + size) * 2);
  }

  memcpy(Data.get() + Offset, data, size);
  Offset += size;
}

void Buffer::Reset()
{
  Offset = 0;
}

void Buffer::Resize(unsigned newSize)
{
  unsigned char* data = Data.release();
  Data.reset(new unsigned char[newSize]);
  Size = newSize;
  memcpy(Data.get(), data, Offset);
  delete[] data;
}

unsigned char* Buffer::GetData()
{
  return Data.get();
}

unsigned Buffer::GetDataSize()
{
  return Offset;
}

unsigned Buffer::GetSize()
{
  return Size;
}

void Buffer::Consolidate(unsigned offset)
{
  unsigned size = Offset - offset;
  Offset = size;

  std::unique_ptr<unsigned char> buf(new unsigned char[size]);
  memcpy(buf.get(), Data.get() + offset, size);
  memset(Data.get(), 0, size);
  memcpy(Data.get(), buf.get(), size);
}