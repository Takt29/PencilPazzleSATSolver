#include <cstring>
#include "MiniSatRunner.h"

MiniSatRunner::MiniSatRunner(std::string command):
  minisatCommand(command) {}

template<typename KeyType>
std::map<KeyType, bool> MiniSatRunner::run(const CNFGenerator<KeyType> &generator, const char *inputPath, const char *outputPath) {
  FILE *input = fopen(inputPath, "w");

  if (input == NULL) {
    fprintf(stderr, "Error: Can't open file.(%s)\n", inputPath);
    exit(1);
  }

  generator.exportCNF(input);
  fclose(input);

  char command[256];
  sprintf(command, "%s %s %s", minisatCommand.c_str(), inputPath, outputPath);
  int status = system(command);

  if (false && status != 0) {
    fprintf(stderr, "Error: Running minisat is failed.\n");
    exit(1);
  }

  FILE *output = fopen(outputPath, "r");
  if (output == NULL) {
    fprintf(stderr, "Error: Can't open file.(%s)\n", outputPath);
    exit(1);
  }

  int variables = generator.getStatus().variables;
  char result[6]; // SAT or UNSAT
  std::vector<bool> literalResult(variables);
  std::map<KeyType, bool> resultDictionary;

  fscanf(output, "%s", result);

  if (strcmp(result, "SAT") != 0) {
    fprintf(stderr, "Can't solve CNF.\n");
    return std::map<KeyType, bool>();
  }

  for (int i=1; i<=variables; i++) {
    int val;
    fscanf(output, "%d", &val);
    literalResult[i] = val > 0;
  }

  for (auto v : generator.getKeyDictionary()) {
    resultDictionary[v.first] = literalResult[v.second];
  }

  return resultDictionary;
}

template std::map<int, bool> MiniSatRunner::run(const CNFGenerator<int>&, const char*, const char*);
template std::map<long long, bool> MiniSatRunner::run(const CNFGenerator<long long>&, const char*, const char*);
template std::map<std::string, bool> MiniSatRunner::run(const CNFGenerator<std::string>&, const char*, const char*);
