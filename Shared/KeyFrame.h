#pragma once

#include <string.h>

const unsigned char KeyFrameData[] = { 0x00, 0x00, 0x00, 0x01, 0x25, 0x88 };
inline bool isKeyFrame(const unsigned char* c)
{
  return memcmp(c, KeyFrameData, 6) == 0;
}