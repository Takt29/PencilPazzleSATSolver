#include <string>
#include "CNFGenerator.h"

template<typename KeyType>
CNFGenerator<KeyType>::CNFGenerator() {

}

template<typename KeyType>
int CNFGenerator<KeyType>::getKeyId(LiteralType literal) {
  int neg = literal.neg ? -1 : 1;
  auto it = keyDictionary.find(literal.key);

  if (it == keyDictionary.end()) {
    int nextKeyId = keyDictionary.size() + 1;
    keyDictionary[literal.key] = nextKeyId;
    return nextKeyId * neg;
  } else {
    return it->second * neg;
  }
}

template<typename KeyType>
void CNFGenerator<KeyType>::addArrayOr(const std::vector<LiteralType> &arr) {
  cnf.push_back(std::vector<int>());
  auto &row = cnf[cnf.size()-1];

  for (auto key : arr) {
    row.push_back(getKeyId(key));
  }
}

template<typename KeyType>
void CNFGenerator<KeyType>::addAtoB(LiteralType a, LiteralType b) {
  cnf.push_back(std::vector<int>());
  auto &row = cnf[cnf.size()-1];

  row.push_back(-getKeyId(a));
  row.push_back(getKeyId(b));
}

template<typename KeyType>
void CNFGenerator<KeyType>::addArrayAndtoB(const std::vector<LiteralType> &arr, LiteralType b) {
  cnf.push_back(std::vector<int>());
  auto &row = cnf[cnf.size()-1];

  for (auto key : arr) {
    row.push_back(-getKeyId(key));
  }
  row.push_back(getKeyId(b));
}

template<typename KeyType>
void CNFGenerator<KeyType>::exportCNF(FILE *file) const {

  // header
  fprintf(file, "p cnf %d %d\n", (int)keyDictionary.size(), (int)cnf.size());

  // body
  for (auto row : cnf) {
    for (auto id : row) {
      fprintf(file, "%d ", id);
    }
    fprintf(file, "0\n");
  }
}

template<typename KeyType>
CNFStatus CNFGenerator<KeyType>::getStatus() const {
  return CNFStatus("cnf", (int)keyDictionary.size(), (int)cnf.size());
}

template<typename KeyType>
std::map<KeyType, int> CNFGenerator<KeyType>::getKeyDictionary() const {
  return keyDictionary;
}

// Explicit Instantiation
template class CNFGenerator<int>;
template class CNFGenerator<long long>;
template class CNFGenerator<std::string>;
