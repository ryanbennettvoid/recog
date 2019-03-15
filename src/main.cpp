
// ref: http://yann.lecun.com/exdb/mnist/

#include "genann.h"
#include <algorithm>
#include <cassert>
#include <math.h>
#include <thread>
#include <time.h>

#include "images_file.h"
#include "labels_file.h"
#include "util.h"

static const std::string TRAINING_IMAGES_FILENAME = "train-images.idx3-ubyte";
static const int TRAINING_IMAGES_MAGIC = 2051;
static const int TRAINING_IMAGES_COUNT = 60000;

static const std::string TRAINING_LABELS_FILENAME = "train-labels.idx1-ubyte";
static const int TRAINING_LABELS_MAGIC = 2049;
static const int TRAINING_LABELS_COUNT = 60000;

static const std::string TEST_IMAGES_FILENAME = "t10k-images.idx3-ubyte";
static const int TEST_IMAGES_MAGIC = 2051;
static const int TEST_IMAGES_COUNT = 10000;

static const std::string TEST_LABELS_FILENAME = "t10k-labels.idx1-ubyte";
static const int TEST_LABELS_MAGIC = 2049;
static const int TEST_LABELS_COUNT = 10000;

static const int THREADS = 8;

const static int INPUTS = 28 * 28;
const static int HIDDEN_LAYERS = 1;
const static int NEURONS = INPUTS / 7;
const static int OUTPUTS = 10;
const static double LEARNING_RATE = 0.1;

int main() {

  double **training_data_input = nullptr;  // training images
  double **training_data_output = nullptr; // training labels
  double **test_data_input = nullptr;      // test images
  double **test_data_output = nullptr;     // test labels

  // outputs:
  // [1, 0, 0, 0, 0, 0, 0, 0, 0, 0] == 0
  // [0, 1, 0, 0, 0, 0, 0, 0, 0, 0] == 1
  // [0, 0, 1, 0, 0, 0, 0, 0, 0, 0] == 2
  // ...

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

    training_data_input = (double **)malloc(numRows * sizeof(double *));
    for (int r = 0; r < numRows; r++) {
      double *row = (double *)malloc(numCols * sizeof(double));
      for (int c = 0; c < numCols; c++) {
        double value = file.images[r]->pixels[c];
        row[c] = value > 0 ? (value / 255) : 0;
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

    int numLabels = file.labels.size();

    training_data_output = (double **)malloc(numLabels * sizeof(double *));
    for (int i = 0; i < numLabels; i++) {
      int index = file.labels[i];
      double *values = (double *)malloc(sizeof(double) * OUTPUTS);
      for (int idx = 0; idx < OUTPUTS; idx++) {
        values[idx] = 0;
      }
      values[index] = 1;
      training_data_output[i] = values;
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

    test_data_input = (double **)malloc(numRows * sizeof(double *));
    for (int r = 0; r < numRows; r++) {
      double *row = (double *)malloc(numCols * sizeof(double));
      for (int c = 0; c < numCols; c++) {
        double value = file.images[r]->pixels[c];
        row[c] = value > 0 ? (value / 255) : 0;
      }
      test_data_input[r] = row;
    }
  }

  // test labels
  {
    auto file = LabelsFile("data/" + TEST_LABELS_FILENAME);
    if (!file.load()) {
      panic("could not load labels file: " + file.getError());
    }
    assert(file.getMagic() == TEST_LABELS_MAGIC);
    assert(file.labels.size() == TEST_LABELS_COUNT);
    file.print();

    int numLabels = file.labels.size();

    test_data_output = (double **)malloc(numLabels * sizeof(double *));
    for (int i = 0; i < numLabels; i++) {
      int index = file.labels[i];
      double *values = (double *)malloc(sizeof(double) * OUTPUTS);
      for (int idx = 0; idx < OUTPUTS; idx++) {
        values[idx] = 0;
      }
      values[index] = 1;
      test_data_output[i] = values;
    }
  }

  assert(training_data_input);
  assert(training_data_output);
  assert(test_data_input);
  assert(test_data_output);

  // inputs, hidden layers, neurons per layer, outputs
  genann *ann = genann_init(INPUTS, HIDDEN_LAYERS, NEURONS, OUTPUTS);

train:

  srand(time(0));

  // shuffle samples
  for (int i = TRAINING_IMAGES_COUNT-1; i > 0; i-- ) {
    int j = rand() % (i+1);
    {
      double* tmp = training_data_input[i];
      training_data_input[j] = training_data_input[i];
      training_data_input[i] = tmp;
    }
    {
      double* tmp = training_data_output[i];
      training_data_output[j] = training_data_output[i];
      training_data_output[i] = tmp;
    }
  }

  // train
  {
    std::vector<std::thread> threads;
    for (int t = 0; t < THREADS; t++) {
      threads.push_back(std::thread(
        [=](int t) {
          int numImages = (TRAINING_IMAGES_COUNT / THREADS);
          int start = numImages * t;
          int end = start + numImages;
          // std::cout << "thread: " << t << '\n';
          // std::cout << "start: " << start << '\n' << '\n';
          int j;
          for (j = start; j < end; ++j) {
              genann_train(ann, training_data_input[j], training_data_output[j], LEARNING_RATE);
            }
          // std::cout << "thread " << t << " done\n\n";
        },
        t));
    }
    for (auto &thread : threads) {
      thread.join();
    }
  }

  // predict
  {
    double guess, guessWeight, actual;
    int i, numGuessesCorrect;
    for (i = 0; i < TEST_IMAGES_COUNT; i++) {
      double const *prediction = genann_run(ann, test_data_input[i]);

      auto guessVec = std::vector<double>(prediction, prediction + OUTPUTS);
      auto actualVec = std::vector<double>(test_data_output[i], test_data_output[i] + OUTPUTS);

      auto maxGuessIterator = std::max_element(guessVec.begin(), guessVec.end());
      guessWeight = *maxGuessIterator;
      guess = std::distance(guessVec.begin(), maxGuessIterator);

      auto maxActualIterator = std::max_element(actualVec.begin(), actualVec.end());
      actual = std::distance(actualVec.begin(), maxActualIterator);

      bool correct = (guess == actual);
      if (correct) {
        numGuessesCorrect++;
      }

      if (i % 1000 == 0) {
        printf("%8d: %d (%f) -> %d (%d/%d correct)\n", i, (int)guess, guessWeight, (int)actual, 
          (int)numGuessesCorrect, (int)TEST_IMAGES_COUNT);
      }
    }
    putchar('\n');
    numGuessesCorrect = 0;
  }

  goto train;

  genann_free(ann);

  return 0;
}