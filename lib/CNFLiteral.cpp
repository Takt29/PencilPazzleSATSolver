#include <string>
#include "CNFLiteral.h"

template<typename KeyType>
CNFLiteral<KeyType>::CNFLiteral(KeyType key, bool neg):
  key(key), neg(neg) {}


// Explicit Instantiation
template class CNFLiteral<int>;
template class CNFLiteral<long long>;
template class CNFLiteral<std::string>;
