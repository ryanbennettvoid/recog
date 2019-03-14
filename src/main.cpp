
#include "images_file.h"
#include "util.h"

static const std::string TRAINING_IMAGES_FILENAME = "train-images.idx3-ubyte";
static const std::string TEST_IMAGES_FILENAME = "t10k-images.idx3-ubyte";

int
main()
{

  // training images
  {
    auto file = ImagesFile("data/" + TRAINING_IMAGES_FILENAME);
    if (!file.load()) {
      panic("could not load images file: " + file.getError());
    }
    assert(file.getMagic() != 0);
    assert(file.images.size() == 60000);
    file.print();
  }

  // test images
  {
    auto file = ImagesFile("data/" + TEST_IMAGES_FILENAME);
    if (!file.load()) {
      panic("could not load images file: " + file.getError());
    }
    assert(file.getMagic() != 0);
    assert(file.images.size() == 10000);
    file.print();
  }

  return 0;
}