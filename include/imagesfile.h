#ifndef IMAGESFILE_H
#define IMAGESFILE_H

#include <string>
#include <vector>

#include "image.h"

class ImagesFile
{
private:
  int magic;
  std::string error;
  std::string filename;
  std::vector<Image*> images;

public:
  ~ImagesFile();
  int getMagic() const;
  std::string getError() const;
  bool load(std::string filename);
  void addImage(Image* image);
  void print();
};

#endif