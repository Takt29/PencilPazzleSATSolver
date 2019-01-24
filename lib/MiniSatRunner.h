#ifndef __MINISAT_RUNNER_H__
#define __MINISAT_RUNNER_H__

#include <string>
#include <map>
#include "CNFGenerator.h"

class MiniSatRunner{
private:
  std::string minisatCommand;

public:
  MiniSatRunner(std::string command = "minisat");

  template<typename KeyType>
    std::map<KeyType, bool> run(const CNFGenerator<KeyType> &generator, const char* inputPath, const char* outputPath);
};

#endif //__MINISAT_RUNNER_H__
