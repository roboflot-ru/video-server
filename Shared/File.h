#pragma once

#include <string>

class File
{
public:
  File(const std::string& name, const std::string& mode = "wb");
  ~File();
  int Write(const void* data, int count);
  int Read(void* data, int count);

private:
  FILE* file;
};

void CopyFile(const std::string& src, const std::string& dst);
void CheckWriteAccess(const std::string& name);
void CheckReadAccess(const std::string& name);
void CreateDirectoryRecursive(const std::string& name);
void ClearDirectory(const std::string& path);
bool IsFileExist(const std::string& filename);
unsigned long GetFileSize(const std::string& filename);
time_t GetFileDate(const std::string& filename);
