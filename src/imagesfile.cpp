
// ref: http://yann.lecun.com/exdb/mnist/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "image.h"
#include "imagesfile.h"

ImagesFile::~ImagesFile()
{
  for (Image* image : this->images) {
    delete image;
  }
}

int
ImagesFile::getMagic() const
{
  return this->magic;
}

std::string
ImagesFile::getError() const
{
  return this->error;
}

bool
ImagesFile::load(std::string filename)
{
  auto file = std::ifstream(filename, std::ios::binary);
  if (!file.good()) {
    this->error = "could not open file: " + filename;
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
        assert(this->magic == 2051);
        assert(numImages == 10000);
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
        currentImage->pixels.push_back(uc);
      }
      pixelPos++;
    }
    pos++;
  }

  assert(!currentImage->pixels.empty());

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
  std::cout << "magic: " << this->magic << '\n';
  std::cout << "error: " << (this->error != "" ? this->error : "<none>")
            << '\n';
  std::cout << "images: " << this->images.size() << '\n';
}
