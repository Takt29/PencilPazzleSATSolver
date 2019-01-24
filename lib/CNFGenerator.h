#ifndef __CNF_GENERATOR_H__
#define __CNF_GENERATOR_H__

#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
#include "CNFLiteral.h"
#include "CNFStatus.h"


template<typename KeyType = int>
class CNFGenerator{
  using LiteralType = CNFLiteral<KeyType>;

private:
  std::vector<std::vector<int> > cnf;
  std::map<KeyType, int> keyDictionary;

  int getKeyId(LiteralType literal);

public:
  CNFGenerator();

  void addArrayOr(const std::vector<LiteralType> &arr);
  void addAtoB(LiteralType a, LiteralType b);
  void addArrayAndtoB(const std::vector<LiteralType> &arr, LiteralType b);

  void exportCNF(FILE* file) const;
  CNFStatus getStatus() const;
  std::map<KeyType, int> getKeyDictionary() const;
};

#endif //__CNF_GENERATOR_H__
