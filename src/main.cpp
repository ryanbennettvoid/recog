
// ref: http://yann.lecun.com/exdb/mnist/

#include "genann.h"

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

  double **training_data_input = nullptr,
         **training_data_output = nullptr /*, **test_data_input*/;

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
        row[c] = file.images[r]->pixels[c];
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
        row[c] = file.labels[r];
      }
      training_data_output[r] = row;
    }
  }

  assert(training_data_input);
  assert(training_data_output);

  // inputs, hidden layer, neurons per layer, outputs
  genann* ann = genann_init(2, 1, 3, 2);

  {
    int i, j;
    for (i = 0; i < 300; ++i) {
      for (j = 0; j < 100; ++j) {
        genann_train(ann, training_data_input[j], training_data_output[j], 0.1);
      }
    }
  }

  // {
  //   double const *prediction = genann_run(ann, test_data_input[i]);
  //   printf("Output for the first test data point is: %f, %f\n",
  //   prediction[0], prediction[1]);
  // }

  // genann_free(ann);

  return 0;
}