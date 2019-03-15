
// ref: http://yann.lecun.com/exdb/mnist/

#include <cassert>
#include "math.h"
#include "genann.h"

#include "images_file.h"
#include "labels_file.h"
#include "util.h"

static const int TRAINING_ITERATIONS = 100;

static const std::string TRAINING_IMAGES_FILENAME = "train-images.idx3-ubyte";
static const int TRAINING_IMAGES_MAGIC = 2051;
static const int TRAINING_IMAGES_COUNT = 60000;

static const std::string TRAINING_LABELS_FILENAME = "train-labels.idx1-ubyte";
static const int TRAINING_LABELS_MAGIC = 2049;
static const int TRAINING_LABELS_COUNT = 60000;
  
static const std::string TEST_IMAGES_FILENAME = "t10k-images.idx3-ubyte";
static const int TEST_IMAGES_MAGIC = 2051;
static const int TEST_IMAGES_COUNT = 10000;

int
main()
{

  double **training_data_input = nullptr, **training_data_output = nullptr,
         **test_data_input = nullptr;

  // training images
  {
    auto file = ImagesFile("data/" + TRAINING_IMAGES_FILENAME);
    if (!file.load()) {
      panic("could not load images file: " + file.getError());
    }
    assert(file.getMagic() == TRAINING_IMAGES_MAGIC);
    assert(file.images.size() == TRAINING_IMAGES_COUNT);
    file.print();

    int numRows = file.images.size();
    int numCols = file.images[0]->pixels.size();

    training_data_input = (double**)malloc(numRows * sizeof(double*));
    for (int r = 0; r < numRows; r++) {
      double* row = (double*)malloc(numCols * sizeof(double));
      for (int c = 0; c < numCols; c++) {
        double value = file.images[r]->pixels[c];
        row[c] = value > 0 ? (value / 255) : 0;
        // std::cout << row[c] << '\n';
      }
      training_data_input[r] = row;
    }
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

    int numRows = file.labels.size();
    int numCols = 1;

    training_data_output = (double**)malloc(numRows * sizeof(double*));
    for (int r = 0; r < numRows; r++) {
      double* row = (double*)malloc(numCols * sizeof(double));
      for (int c = 0; c < numCols; c++) {
        double value = file.labels[r];
        row[c] = value > 0 ? (value / 10) : 0;
      }
      training_data_output[r] = row;
    }
  }

  assert(training_data_input);
  assert(training_data_output);

  // inputs, hidden layers, neurons per layer, outputs
  genann* ann = genann_init(28*28, 1, 28*28, 10);

  {
    int i, j;
    for (i = 0; i < TRAINING_ITERATIONS; ++i) {
      for (j = 0; j < TRAINING_IMAGES_COUNT; ++j) {
        genann_train(ann, training_data_input[j], training_data_output[j], 0.1);
      }
    }
  }

  // test images
  {
    auto file = ImagesFile("data/" + TEST_IMAGES_FILENAME);
    if (!file.load()) {
      panic("could not load images file: " + file.getError());
    }
    assert(file.getMagic() == TEST_IMAGES_MAGIC);
    assert(file.images.size() == TEST_IMAGES_COUNT);
    file.print();

    int numRows = file.images.size();
    int numCols = file.images[0]->pixels.size();

    test_data_input = (double**)malloc(numRows * sizeof(double*));
    for (int r = 0; r < numRows; r++) {
      double* row = (double*)malloc(numCols * sizeof(double));
      for (int c = 0; c < numCols; c++) {
        double value = file.images[r]->pixels[c];
        row[c] = value > 0 ? (value / 255) : 0;
      }
      test_data_input[r] = row;
    }
  }

  assert(test_data_input);

  {
    int i;
    for (i = 0; i < TEST_IMAGES_COUNT; i++) {
      double const* prediction = genann_run(ann, test_data_input[i]);
      printf(
        "Output for data point is: %f, %f\n", prediction[0], prediction[1]);
    }
  }

  genann_free(ann);

  return 0;
}