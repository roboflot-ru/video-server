#include "File.h"
#include "dirent.h"
#include "stdio.h"
#include "sys/stat.h"
#include <stdexcept>


File::File(const std::string& name, const std::string& mode)
{
  file = fopen(name.c_str(), mode.c_str());
  if (!file) 
  {
    throw std::runtime_error("Failed to open file " + name + ". Error - " + std::to_string(errno));
  }
}

int File::Write(const void* data, int count)
{
  return fwrite(data, 1, count, file);
}

int File::Read(void* data, int count)
{
  return fread(data, 1, count, file);
}

File::~File()
{
  fclose(file);
}

const unsigned CopyBufSize = 1024 * 64;

void CopyFile(const std::string& src, const std::string& dst)
{
  File srcFile(src, "rb");
  File dstFile(dst);

  char buf[CopyBufSize];

  while(1)
  {
    int size = srcFile.Read(buf, CopyBufSize);
    dstFile.Write(buf, size);
    if (size != CopyBufSize)
    {
      break;
    }
  }
}

void CheckWriteAccess(const std::string& name)
{
  File file(name);
  char c = '0';
  if (file.Write(&c, 1) != 1)
  {
    throw std::runtime_error("Failed to write to file " + name);
  }
}

void CheckReadAccess(const std::string& name)
{
  File file(name, "r");
}

void CreateDirectory(const std::string& name)
{
  if (mkdir(name.c_str(), 0755) == -1)
  {
    throw std::runtime_error("Failed to create directory " + name);
  }
}

void CreateDirectoryRecursive(const std::string& name)
{
  struct stat info;
  if (stat(name.c_str(), &info) != -1 && info.st_mode & S_IFDIR)
  {
    return;
  }

  std::size_t found = name.rfind('/');
  if (found != std::string::npos)
  {
    CreateDirectoryRecursive(name.substr(0, found));
  }

  CreateDirectory(name);
}

void ClearDirectory(const std::string& path)
{
  DIR* dir = opendir(path.c_str());
  if (!dir)
  {
    return;
  }

  while (dirent* entry = readdir(dir))
  {
    if (!entry->d_name || std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
    {
      continue;
    }

    std::string fullpath = path + "/" + entry->d_name;
    if (entry->d_type & DT_DIR)
    {
      ClearDirectory(fullpath); 
    }
    std::remove(fullpath.c_str());
  }
}

bool IsFileExist(const std::string& filename)
{
  struct stat info;
  int ret = stat(filename.c_str(), &info);
  if (ret == -1)
  {
    return errno != ENOENT;
  }
  return ret != ENOENT;    
}

unsigned long GetFileSize(const std::string& filename)
{
  struct stat info;
  stat(filename.c_str(), &info);
  return info.st_size;
}

time_t GetFileDate(const std::string& filename)
{
  struct stat info;
  stat(filename.c_str(), &info);
  return info.st_mtim.tv_sec;
}
