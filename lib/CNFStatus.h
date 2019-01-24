#ifndef __CNF_STATUS_H__
#define __CNF_STATUS_H__

#include <string>

class CNFStatus{
public:
  std::string format;
  int variables;
  int clauses;

  CNFStatus(std::string format, int variables, int clauses);
};

#endif //__CNF_STATUS_H__
