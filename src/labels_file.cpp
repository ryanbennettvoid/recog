
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "labels_file.h"

LabelsFile::LabelsFile(std::string filename)
  : File(filename)
{}

LabelsFile::~LabelsFile() {}

bool
LabelsFile::load()
{
  auto file = std::ifstream(this->filename, std::ios::binary);
  if (!file.good()) {
    this->error = "could not open file: " + this->filename;
    return false;
  }

  int pos = 0, labelPos = 0, numLabels = 0;

  char c;
  unsigned char uc;
  while (file.get(c)) {
    uc = (unsigned char)c;
    if (pos < 4) {
      this->magic += (uc << ((3 - pos) * 8));
    } else if (pos < 8) {
      numLabels += (uc << ((3 - pos) * 8));
    } else {
      this->labels.push_back(uc);
      labelPos++;
    }
    pos++;
  }

  file.close();
  return true;
}

void
LabelsFile::addLabel(unsigned char label)
{
  this->labels.push_back(label);
}

void
LabelsFile::print()
{
  std::cout << "filename: " << this->filename << '\n';
  std::cout << "magic: " << this->magic << '\n';
  std::cout << "error: " << (this->error != "" ? this->error : "<none>")
            << '\n';
  std::cout << "labels: " << this->labels.size() << '\n';
  std::cout << '\n';
}
