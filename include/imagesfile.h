#ifndef IMAGESFILE_H
#define IMAGESFILE_H

#include <string>
#include <vector>

#include "file.h"
#include "image.h"

class ImagesFile : public File
{
private:
  std::vector<Image*> images;

public:
  ImagesFile(std::string filename);
  ~ImagesFile();

  bool load();
  void print();
  void addImage(Image* image);
};

#endif