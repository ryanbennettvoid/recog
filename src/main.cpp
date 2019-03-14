
// ref: http://yann.lecun.com/exdb/mnist/

#include "images_file.h"
#include "labels_file.h"
#include "util.h"

static const std::string TRAINING_IMAGES_FILENAME = "train-images.idx3-ubyte";
static const int TRAINING_IMAGES_MAGIC = 2051;
static const int TRAINING_IMAGES_COUNT = 60000;

static const std::string TRAINING_LABELS_FILENAME = "train-labels.idx1-ubyte";
static const int TRAINING_LABELS_MAGIC = 2049;
static const int TRAINING_LABELS_COUNT = 60000;

int
main()
{

  // training images
  {
    auto file = ImagesFile("data/" + TRAINING_IMAGES_FILENAME);
    if (!file.load()) {
      panic("could not load images file: " + file.getError());
    }
    assert(file.getMagic() == TRAINING_IMAGES_MAGIC);
    assert(file.images.size() == TRAINING_IMAGES_COUNT);
    file.print();
  }

  // training labels
  {
    auto file = LabelsFile("data/" + TRAINING_LABELS_FILENAME);
    if (!file.load()) {
      panic("could not load labels file: " + file.getError());
    }
    assert(file.getMagic() == TRAINING_LABELS_MAGIC);
    assert(file.labels.size() == TRAINING_LABELS_COUNT);
    file.print();
  }

  return 0;
}