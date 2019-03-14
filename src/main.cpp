
#include "imagesfile.h"
#include "util.h"

int
main(int argc, char** argv)
{

  if (argc != 2) {
    panic("invalid number of arguments");
  }

  auto filename = std::string(argv[1]);

  ImagesFile file;
  if (!file.load(filename)) {
    panic("could not load images file: " + file.getError());
  }

  assert(file.getMagic() != 0);

  file.print();

  return 0;
}