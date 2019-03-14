#ifndef UTIL_H
#define UTIL_H

#include <iostream>

void
panic(std::string message)
{
  std::cout << message << "\n";
  exit(1);
}

#endif