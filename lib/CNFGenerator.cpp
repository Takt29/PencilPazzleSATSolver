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
void CNFGenerator<KeyType>::addA(LiteralType a) {
  cnf.push_back(std::vector<int>{getKeyId(a)});
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
  cnf.push_back(std::vector<int>{-getKeyId(a), getKeyId(b)});
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
void CNFGenerator<KeyType>::addGreaterThanOrEqualX(const std::vector<LiteralType> &arr, int x){
  int N = arr.size();

  if (N >= 20) {
    fprintf(stderr, "[addGreaterThanOrEqualX] Array size is must be less then 20.");
    exit(1);
  }

  std::vector<int> comb(N);
  for (int i=0; i<N; i++) {
    comb[i] = i >= x-1;
  }

  do{
    std::vector<LiteralType> combArr;
    for (int i=0; i<N; i++)
      if (comb[i])
        combArr.push_back(arr[i]);

    addArrayOr(combArr);
  }while(next_permutation(comb.begin(), comb.end()));
}

template<typename KeyType>
void CNFGenerator<KeyType>::addLessThanOrEqualX(const std::vector<LiteralType> &arr, int x){
  int N = arr.size();

  if (N >= 20) {
    fprintf(stderr, "[addLessThanOrEqualX] Array size is must be less then 20.");
    exit(1);
  }

  std::vector<int> comb(N);
  std::vector<LiteralType> negativeArr(N, LiteralType(0, false));
  for (int i=0; i<N; i++) {
    comb[i] = N-i-1 < x+1;
    negativeArr[i] = arr[i];
    negativeArr[i].neg = !negativeArr[i].neg;
  }

  do{
    std::vector<LiteralType> combArr;
    for (int i=0; i<N; i++)
      if (comb[i])
        combArr.push_back(negativeArr[i]);

    addArrayOr(combArr);
  }while(next_permutation(comb.begin(), comb.end()));
}

template<typename KeyType>
void CNFGenerator<KeyType>::addEqualX(const std::vector<LiteralType> &arr, int x) {
  addLessThanOrEqualX(arr, x);
  addGreaterThanOrEqualX(arr, x);
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
