#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>

#include "file.h"

class File
{
protected:
  int magic;
  std::string error;
  std::string filename;

public:
  File(std::string filename);
  ~File();

  int getMagic() const;
  std::string getError() const;
  bool load();
  void print();
};

#endif