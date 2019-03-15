
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include "genann.h"
#include "util.h"

const static int OUTPUTS = 10;
const static std::string OUTPUT_FILE = "out.nn";

cv::Mat& increaseContrast(cv::Mat& image)
{
  cv::Mat newMat;
  double contrast = 1;
  int brightness = 3;

  for( int y = 0; y < image.rows; y++ ) {
    for( int x = 0; x < image.cols; x++ ) {
      for( int c = 0; c < image.channels(); c++ ) {
          newMat.at<cv::Vec3b>(y,x)[c] =
            cv::saturate_cast<uchar>( contrast*image.at<cv::Vec3b>(y,x)[c] + brightness );
      }
    }
  }
  return newMat;
}

int main()
{

  // initialize stream
  cv::VideoCapture videoStream(0);
  if (!videoStream.isOpened())
  {
    puts("could not open video capture");
    return 1; 
  }
  
  cv::namedWindow("cam", 1 );
  cv::namedWindow("final", 1 );
  
  cv::Size smallSize(28, 28);

  genann *ann = nullptr;

  FILE* fileToLoad = fopen(OUTPUT_FILE.c_str(), "r");
  if (fileToLoad && (ann = genann_read(fileToLoad))) {
      fclose(fileToLoad);
    } else {
      panic("could not load file");
  }
  
  double* inputData = (double*)malloc(sizeof(double) * 28*28);

  int count = 0;
  while (true) {
    cv::Mat sourceFrame;
    cv::Mat destFrame;
    videoStream >> sourceFrame;
    cv::resize(sourceFrame, destFrame, smallSize);
    cvtColor(destFrame, destFrame, cv::COLOR_BGR2GRAY);
    cv::imshow("cam", sourceFrame);
    cv::imshow("final", destFrame);

    increaseContrast(destFrame);

    int size = 28;
    for (int r = 0; r < size; r++) {
      for (int c = 0; c < size; c++) {
        double pixel = (double)destFrame.at<unsigned char>(r, c) / 255;
        int idx = (r * size) + c;
        inputData[idx] = pixel;
        // std::cout << pixel << "\n";
      }
    }

    if (count++ % 30 == 0) {
      double const *prediction = genann_run(ann, inputData);
      auto guessVec = std::vector<double>(prediction, prediction + OUTPUTS);
      auto maxGuessIterator = std::max_element(guessVec.begin(), guessVec.end());
      double guessWeight = *maxGuessIterator;
      double guess = std::distance(guessVec.begin(), maxGuessIterator);
      printf("%f (%f)\n", guess, guessWeight);
    }

    if(cv::waitKey(30) >= 0) break;
  }

  free(inputData);

  return 0;
}