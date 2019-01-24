#ifndef __CNF_LITERAL_H__
#define __CNF_LITERAL_H__

template<typename KeyType>
class CNFLiteral{
public:
  KeyType key;
  bool neg;

  CNFLiteral(KeyType key, bool neg = false);
};

#endif //__CNF_LITERAL_H__
