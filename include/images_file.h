#ifndef IMAGES_FILE_H
#define IMAGES_FILE_H

#include <string>
#include <vector>

#include "file.h"
#include "image.h"

class ImagesFile : public File
{
public:
  ImagesFile(std::string filename);
  ~ImagesFile();

  bool load();
  void print();

  void addImage(Image* image);
  std::vector<Image*> images;
};

#endif