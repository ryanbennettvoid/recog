
#include <iostream>
#include <string>
#include "math.h"

void
panic(std::string message)
{
  std::cout << message << "\n";
  exit(1);
}
