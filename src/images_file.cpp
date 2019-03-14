
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "image.h"
#include "images_file.h"

ImagesFile::ImagesFile(std::string filename)
  : File(filename)
{}

ImagesFile::~ImagesFile()
{
  for (Image* image : this->images) {
    delete image;
  }
}

bool
ImagesFile::load()
{
  auto file = std::ifstream(this->filename, std::ios::binary);
  if (!file.good()) {
    this->error = "could not open file: " + this->filename;
    return false;
  }

  int pos = 0, pixelPos = 0, numImages = 0, numRows = 0, numColumns = 0,
      numPixels = 0;

  Image* currentImage = nullptr;
  char c;
  unsigned char uc;
  while (file.get(c)) {
    uc = (unsigned char)c;
    if (pos < 4) {
      this->magic += (uc << ((3 - pos) * 8));
    } else if (pos < 8) {
      numImages += (uc << ((3 - pos) * 8));
    } else if (pos < 12) {
      numRows += (uc << ((3 - pos) * 8));
    } else if (pos < 16) {
      numColumns += (uc << ((3 - pos) * 8));
    } else {
      if (pos == 16) {
        assert(numRows == 28);
        assert(numColumns == 28);
        numPixels = numRows * numColumns;
      }
      // printf("%4d,", uc);
      if (pixelPos % numColumns == 0) {
        // next row
        // putchar('\n');
      }
      if (pixelPos % numPixels == 0) {
        // next image
        // putchar('\n');
        if (pixelPos > 0) {
          assert(currentImage);
          this->images.push_back(currentImage);
        }
        currentImage = new Image();
      }
      currentImage->pixels.push_back(uc);
      pixelPos++;
    }
    pos++;
  }

  // last image
  assert(!currentImage->pixels.empty());
  this->images.push_back(currentImage);

  file.close();
  return true;
}

void
ImagesFile::addImage(Image* image)
{
  this->images.push_back(image);
}

void
ImagesFile::print()
{
  std::cout << "filename: " << this->filename << '\n';
  std::cout << "magic: " << this->magic << '\n';
  std::cout << "error: " << (this->error != "" ? this->error : "<none>")
            << '\n';
  std::cout << "images: " << this->images.size() << '\n';
  std::cout << '\n';
}
