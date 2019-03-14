
#include <iostream>
#include <string>

void
panic(std::string message)
{
  std::cout << message << "\n";
  exit(1);
}