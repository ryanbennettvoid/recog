#ifndef LABELS_FILE_H
#define LABELS_FILE_H

#include <string>
#include <vector>

#include "file.h"

class LabelsFile : public File
{
public:
  LabelsFile(std::string filename);
  ~LabelsFile();

  bool load();
  void print();

  void addLabel(unsigned char label);
  std::vector<unsigned char> labels;
};

#endif