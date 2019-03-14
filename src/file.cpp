
// ref: http://yann.lecun.com/exdb/mnist/

#include <iostream>
#include <string>

#include "file.h"
#include "util.h"

File::File(std::string filename)
  : filename(filename)
{}

File::~File() {}

int
File::getMagic() const
{
  return this->magic;
}

std::string
File::getError() const
{
  return this->error;
}

bool
File::load()
{
  panic("::load is NOT implemented");
  return true;
}

void
File::print()
{
  std::cout << "filename: " << this->filename << '\n';
  std::cout << "magic: " << this->magic << '\n';
  std::cout << "error: " << (this->error != "" ? this->error : "<none>")
            << '\n';
  std::cout << '\n';
}
